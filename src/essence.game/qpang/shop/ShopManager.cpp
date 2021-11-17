#include "ShopManager.h"

#include <iostream>

#include "qpang/player/Player.h"
#include "qpang/player/inventory/InventoryManager.h"
#include "qpang/player/inventory/InventoryCard.h"

#include "packets/lobby/outgoing/shop/SendShopCardPurchaseComplete.h"
#include "packets/lobby/outgoing/shop/SendShopItems.h"

void ShopManager::initialize()
{
	m_items.clear();
	m_orderedItems.clear();

	const auto stmt = DATABASE->prepare("SELECT * FROM `items` ORDER BY seq_id");
	const auto res = stmt->fetch();

	std::cout << "Loading shop items.\n";

	while (res->hasNext())
	{
		ShopItem item{
			res->getInt("seq_id"),
			res->getInt("item_id"),
			res->getFlag("pay_type"),
			res->getInt("price"),
			res->getInt("sold_count"),
			res->getInt("stock"),
			res->getTiny("status"),
			res->getTiny("type"),
			res->getTiny("use_up"),
			res->getShort("period"),
			res->getShort("level"),
		};

		m_items[item.seqId] = item;
		m_orderedItems.push_back(item);

		res->next();
	}

	std::cout << "Loaded " << m_items.size() << " items.\n";
}

std::vector<ShopItem> ShopManager::list()
{
	std::vector<ShopItem> items{};

	for (const auto& [id, item] : m_items)
	{
		items.push_back(item);
	}

	return items;
}

void ShopManager::sendShopItems(const Player::Ptr& player)
{
	// list of every element. the size of this list = the first short
	const std::vector<ShopItem> items = list();

	// the amount of elements after which the packet will be divided. can be configured
	constexpr uint16_t partitionSize = 75;

	// the amount of partitions to send
	const auto partitionCount = static_cast<uint32_t>(ceil(items.size() / static_cast<double>(partitionSize)));

	for (uint16_t i = 0; i < partitionCount; i++)
	{
		// this value = the second short
		const uint16_t currentSendCount = i * partitionSize + partitionSize >= items.size()
			? static_cast<uint16_t>(items.size())
			: i * partitionSize + partitionSize;

		// create a sublist. the size of this = the third short
		std::vector<ShopItem> partition = slice(items, i * partitionSize + 1, currentSendCount);

		// now send the data
		player->send(SendShopItems(partition, static_cast<uint16_t>(items.size()), currentSendCount, static_cast<uint16_t>(partition.size())));
	}
}

ShopItem ShopManager::get(const uint32_t seqId)
{
	auto it = m_items.find(seqId);

	return it != m_items.cend() ? (*it).second : ShopItem{};
}

bool ShopManager::exists(const uint32_t seqId)
{
	return m_items.find(seqId) != m_items.cend();
}

InventoryCard ShopManager::buy(const std::shared_ptr<Player>& player, const uint32_t seqId, const bool updateShopItems)
{
	// TODO: Allow players to buy shop packages.
	assert(player != nullptr);

	if (!exists(seqId))
	{
		return InventoryCard{};
	}

	if (!player->getInventoryManager()->hasSpace())
	{
		return InventoryCard{};
	}

	const auto& shopItem = get(seqId);

	if (shopItem.soldCount >= shopItem.stock && shopItem.stock != 9999)
	{
		return InventoryCard{};
	}

	const auto money = shopItem.isCash ? player->getCash() : player->getDon();

	if (const auto hasEnough = money >= shopItem.price; !hasEnough)
	{
		return InventoryCard{};
	}

	auto card = InventoryCard::fromShopItem(shopItem);

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

	ShopItem* itemPtr = &m_items[seqId];
	itemPtr->soldCount++;

	const auto stmt2 = DATABASE->prepare("UPDATE items SET sold_count = ? WHERE seq_id = ?");

	stmt2->bindInteger(itemPtr->soldCount);
	stmt2->bindInteger(itemPtr->seqId);

	stmt2->execute();

	const auto cardId = stmt->getLastInsertId();

	card.id = cardId;
	card.playerOwnerId = player->getId();

	shopItem.isCash ? player->removeCash(shopItem.price) : player->removeDon(shopItem.price);

	player->getInventoryManager()->addCard(card);

	if (updateShopItems)
	{
		sendShopItems(player);
		player->send(SendShopCardPurchaseComplete(shopItem, { card }, shopItem.isCash ? player->getCash() : player->getDon()));
	}

	return card;
}
