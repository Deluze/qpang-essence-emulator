#include "InventoryManager.h"

#include "OpenCardSuccess.h"
#include "OpenGift.h"
#include "qpang/Game.h"
#include "qpang/player/Player.h"

#include "packets/lobby/outgoing/inventory/RemoveCard.h"
#include "packets/lobby/outgoing/inventory/DisableFunctionCard.h"
#include "packets/lobby/outgoing/inventory/EnableFunctionCard.h"
#include "packets/lobby/outgoing/inventory/ReceiveGift.h"
#include "packets/lobby/outgoing/inventory/GiftCardSuccess.h"
#include "packets/lobby/outgoing/inventory/OpenGiftSuccess.h"

#include "qpang/ItemId.h"

void InventoryManager::initialize(std::shared_ptr<Player> player, uint32_t playerId)
{
	assert(player != nullptr);

	m_player = player;

	Statement::Ptr stmt = DATABASE->prepare("SELECT * FROM player_items WHERE player_id = ? AND ((type = 75 AND period > 0) OR (type != 75)) ORDER BY time");
	stmt->bindInteger(playerId);
	StatementResult::Ptr res = stmt->fetch();

	std::vector<uint64_t> functionCardIds;
	std::vector<uint64_t> skillCardIds;

	while (res->hasNext())
	{
		InventoryCard card
		{
			res->getBigInt("id"),
			res->getInt("player_id"),
			res->getInt("item_id"),
			res->getTiny("type"),
			res->getTiny("period_type"),
			res->getShort("period"),
			res->getFlag("active"),
			res->getFlag("opened"),
			res->getFlag("giftable"),
			res->getTiny("boost_level"),
			res->getInt("time")
		};

		if (card.isOpened)
		{
			m_cards[card.id] = card;
		}
		else
		{
			m_gifts[card.id] = card;
		}

		// Must be a function card, period can not be 1 and card must be active.
		if (card.type == 70 && card.periodType != 1 && card.isActive)
		{
			functionCardIds.push_back(card.id);
		}

		if (card.type == 75 && card.isActive)
		{
			if (skillCardIds.size() < 3)
			{
				skillCardIds.push_back(card.id);
			}
		}

		res->next();
	}

	std::cout << "Player has the following gifts:" << std::endl;

	for (const auto& gift : m_gifts)
	{
		std::cout << "- " << gift.second.id << std::endl;
	}

	player->getEquipmentManager()->setFunctionCards(functionCardIds);
	player->getEquipmentManager()->setSkillCardIds(skillCardIds);
}

std::vector<InventoryCard> InventoryManager::list()
{
	std::lock_guard<std::mutex> lg(m_mx);

	std::vector<InventoryCard> cards;

	for (const auto& [key, val] : m_cards)
		cards.push_back(val);

	return cards;
}

std::vector<InventoryCard> InventoryManager::listGifts()
{
	std::lock_guard<std::mutex> lg(m_mx);

	std::vector<InventoryCard> cards;

	for (const auto& [key, card] : m_gifts)
		cards.push_back(card);

	return cards;
}

InventoryCard InventoryManager::get(uint64_t cardId)
{
	const auto it = m_cards.find(cardId);

	return it != m_cards.cend() ? it->second : InventoryCard{};
}

bool InventoryManager::hasCard(const uint64_t cardId)
{
	std::lock_guard<std::mutex> lg(m_mx);

	const auto it = m_cards.find(cardId);

	return it != m_cards.cend();
}

void InventoryManager::deleteCard(const uint64_t cardId)
{
	if (!hasCard(cardId))
		return;

	std::lock_guard<std::mutex> lg(m_mx);

	if (const auto player = m_player.lock(); player != nullptr)
	{
		const auto isCurrentlyEquipped = player->getEquipmentManager()->hasEquipped(cardId);

		if (isCurrentlyEquipped)
			return;

		m_cards.erase(cardId);

		const auto stmt = DATABASE->prepare("DELETE FROM player_items WHERE id = ? AND player_id = ?");
		stmt->bindLong(cardId);
		stmt->bindInteger(player->getId()); // for security
		stmt->execute();

		player->send(RemoveCard(cardId));
	}
}

void InventoryManager::setCardActive(const uint64_t cardId, bool isActive)
{
	std::lock_guard<std::mutex> lg(m_mx);

	if (const auto player = m_player.lock(); player != nullptr)
	{
		const auto equipmentManager = player->getEquipmentManager();

		const auto& it = m_cards.find(cardId);

		if (it == m_cards.cend())
		{
			return;
		}

		auto& card = it->second;

		if (card.type == 70)
		{
			const auto alreadyHasSameFunctionCard = equipmentManager->hasFunctionCard(card.itemId);

			if (isActive && !alreadyHasSameFunctionCard)
			{
				card.timeCreated = time(nullptr);
				card.isActive = true;
				equipmentManager->addFunctionCard(cardId);
				player->send(EnableFunctionCard(card));
			}
			else if (!isActive && alreadyHasSameFunctionCard)
			{
				card.isActive = false;
				equipmentManager->removeFunctionCard(cardId);
				player->send(DisableFunctionCard(card));
			}
		}
		else if (card.type == 75)
		{
			card.isActive = isActive;
		}

		m_cards[card.id] = card;
	}
}

bool InventoryManager::addCard(const InventoryCard& card)
{
	if (!hasSpace())
		return false;

	std::lock_guard lg(m_mx);

	m_cards[card.id] = card;

	return true;
}

InventoryCard InventoryManager::storeCard(InventoryCard& card)
{
	const auto player = m_player.lock();

	if (player == nullptr)
	{
		return InventoryCard{};
	}

	const auto stmt = DATABASE->prepare(
		"INSERT INTO player_items (player_id, item_id, period, period_type, type, active, opened, giftable, boosted, boost_level, time)" \
		"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

	stmt->bindInteger(player->getId());
	stmt->bindInteger(card.itemId);
	stmt->bindInteger(card.period);
	stmt->bindInteger(card.periodType);
	stmt->bindInteger(card.type);
	stmt->bindInteger(card.isActive);
	stmt->bindInteger(card.isOpened);
	stmt->bindInteger(card.isGiftable);
	stmt->bindInteger(card.boostLevel > 0);
	stmt->bindInteger(card.boostLevel);
	stmt->bindLong(time(nullptr));

	stmt->execute();

	const auto cardId = stmt->getLastInsertId();

	card.id = cardId;
	card.timeCreated = time(nullptr);
	card.playerOwnerId = player->getId();

	addCard(card);

	return card;
}

void InventoryManager::useSkillCard(const uint64_t cardId, const uint16_t period)
{
	std::lock_guard lg(m_mx);

	const auto player = m_player.lock();

	if (player == nullptr)
	{
		return;
	}

	const auto& it = m_cards.find(cardId);

	if (it == m_cards.cend())
	{
		return;
	}

	auto& card = it->second;

	if (card.periodType == 254)
	{
		return;
	}

	if (card.type != 75 || card.periodType != 1)
	{
		return;
	}

	card.period = period;

	if (card.period <= 0)
	{
		card.isActive = false;

		player->getEquipmentManager()->removeSkillCard(cardId);
	}

	DATABASE_DISPATCHER->dispatch("UPDATE player_items SET period = ?, active = ? WHERE id = ?",
		{
			card.period,
			card.isActive,
			card.id
		});
}

void InventoryManager::useCard(const uint64_t cardId, const uint32_t period)
{
	std::lock_guard lg(m_mx);

	const auto player = m_player.lock();

	if (player == nullptr)
	{
		return;
	}

	const auto it = m_cards.find(cardId);

	if (it == m_cards.cend())
	{
		return;
	}

	auto& card = it->second;

	const auto periodType = it->second.periodType;

	if (periodType == 254)
	{
		return;
	}

	// Skillcards do not get handled here.
	if (card.type == 75)
	{
		return;
	}

	// Rounds or uses.
	if (periodType == 3)
	{
		if (card.period > 0)
		{
			card.period--;
		}
	}
	else if (periodType == 2)
	{
		card.period = card.period <= period ? 0 : card.period - period;
	}

	if (periodType != 254)
	{
		DATABASE_DISPATCHER->dispatch("UPDATE player_items SET period = IF(period_type = 3, period - 1, period - ?) "\
			"WHERE id = ?"
			, {
				period,
				cardId
			});
	}

	if (it->second.period <= 0)
	{
		if (card.type == 86 || card.type == 87)
		{
			player->getEquipmentManager()->unequipItem(cardId);
		}
		else if (card.type == 70)
		{
			card.isActive = false;
			player->getEquipmentManager()->removeFunctionCard(cardId);
		}
	}
}

bool InventoryManager::isExpired(const uint64_t cardId)
{
	std::lock_guard<std::mutex> lg(m_mx);

	const auto it = m_cards.find(cardId);

	if (it == m_cards.cend())
		return true;

	return it->second.period == 0;
}

bool InventoryManager::hasSpace(const uint32_t space) const
{
	return m_cards.size() + m_gifts.size() + space < 200;
}

bool InventoryManager::hasSpace() const
{
	return m_cards.size() + m_gifts.size() < 200;
}

void InventoryManager::giftCard(InventoryCard& card, const std::shared_ptr<Player>& player)
{
	const auto our = m_player.lock();

	if (our == nullptr)
		return;

	std::lock_guard<std::mutex> lg(m_mx);

	auto cardOwnerPlayer = Game::instance()->getPlayer(card.playerOwnerId);

	m_cards.erase(card.id);
	card.timeCreated = time(nullptr);
	player->getInventoryManager()->receiveGift(card, our->getName());

	DATABASE_DISPATCHER->dispatch(
		"UPDATE player_items SET player_id = ?, opened = 0, time = ? WHERE id = ?",
		{
			player->getId(),
			card.timeCreated,
			card.id
		}
	);

	DATABASE_DISPATCHER->dispatch(
		"INSERT INTO player_gifts (player_from_id, player_from_name, player_to_id, player_to_name, card_id) VALUES(?, ?, ?, ?, ?)",
		{
			cardOwnerPlayer->getId(),
			cardOwnerPlayer->getName(),
			player->getId(),
			player->getName(),
			card.id,
		}
	);

	our->send(GiftCardSuccess(card.id));

	our->getEquipmentManager()->save();
}

void InventoryManager::receiveGift(InventoryCard& card, const std::u16string& sender)
{
	auto player = m_player.lock();

	if (player == nullptr)
		return;

	std::lock_guard<std::mutex> lg(m_mx);

	card.isOpened = false;
	card.playerOwnerId = player->getId();
	m_gifts[card.id] = card;

	player->send(ReceiveGift(card, sender));
}

void InventoryManager::openGift(uint64_t cardId)
{
	const auto player = m_player.lock();

	if (player == nullptr)
	{
		return;
	}

	std::lock_guard<std::mutex> lg(m_mx);

	const auto it = m_gifts.find(cardId);

	if (it == m_gifts.cend())
	{
		return;
	}

	auto card = it->second;
	card.isOpened = true;
	m_cards[card.id] = card;

	m_gifts.erase(it);

	DATABASE_DISPATCHER->dispatch("UPDATE player_items SET opened = 1 WHERE id = ?", { card.id });

	player->send(OpenGiftSuccess(player, card));
}

bool InventoryManager::hasGiftSpace() const
{
	return m_gifts.size() < 5;
}

void InventoryManager::openCard(const uint64_t cardId)
{
	const auto player = m_player.lock();

	if (player == nullptr)
	{
		return;
	}

	std::lock_guard<std::mutex> lg(m_mx);

	const auto it = m_cards.find(cardId);

	if (it == m_cards.cend())
	{
		return;
	}

	auto card = it->second;

	card.isOpened = true;

	m_cards[card.id] = card;

	DATABASE_DISPATCHER->dispatch("UPDATE player_items SET opened = 1 WHERE id = ?", { card.id });

	player->send(OpenCardSuccess(cardId));
}

void InventoryManager::close()
{
	std::lock_guard<std::mutex> lg(m_mx);

	for (const auto& [id, card] : m_cards)
	{
		if (isEquippableFunction(card.itemId) || card.type == 75)
		{
			DATABASE_DISPATCHER->dispatch("UPDATE player_items SET active = ? WHERE id = ?", { card.isActive, card.id });
		}
	}
}
