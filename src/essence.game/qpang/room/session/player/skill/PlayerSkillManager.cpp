#include "PlayerSkillManager.h"

#include "cg_card.hpp"
#include "qpang/Game.h"

#include "qpang/player/Player.h"
#include "qpang/room/session/player/RoomSessionPlayer.h"
#include "qpang/player/equipment/EquipmentManager.h"

#include "qpang/room/tnl/net_events/server/gc_card.hpp"

void PlayerSkillManager::initialize(const std::shared_ptr<RoomSessionPlayer>& player)
{
	m_player = player;

	setupEquippedSkillCards();
	initializeSkillGaugeBoosters();
}

void PlayerSkillManager::setupEquippedSkillCards()
{
	if (const auto roomSessionPlayer = m_player.lock(); roomSessionPlayer != nullptr)
	{
		const auto player = roomSessionPlayer->getPlayer();
		const auto equippedSkillCardIds = player->getEquipmentManager()->getEquippedSkillCardIds();

		for (size_t i = 0; i < equippedSkillCardIds.size() && i < 3; i++)
		{
			const auto cardId = equippedSkillCardIds[i];

			if (cardId == 0)
			{
				continue;
			}

			const auto [id, playerOwnerId, itemId, type, periodType, period, isActive, isOpened, isGiftable, boostLevel,
				timeCreated] = player->getInventoryManager()->get(cardId);

			if (i == 0)
			{
				m_firstSkillCard = roomSessionPlayer->getRoomSession()->getSkillManager()->getSkillByItemId(itemId);

				if (m_firstSkillCard != nullptr)
				{
					m_firstSkillCard->setUsesLeftCount(period);
					m_firstSkillCard->bind(roomSessionPlayer);
				}

			}
			else if (i == 1)
			{
				m_secondSkillCard = roomSessionPlayer->getRoomSession()->getSkillManager()->getSkillByItemId(itemId);

				if (m_secondSkillCard != nullptr)
				{
					m_secondSkillCard->setUsesLeftCount(period);
					m_secondSkillCard->bind(roomSessionPlayer);
				}

			}
			else if (i == 2)
			{
				m_thirdSkillCard = roomSessionPlayer->getRoomSession()->getSkillManager()->getSkillByItemId(itemId);

				if (m_thirdSkillCard != nullptr)
				{
					m_thirdSkillCard->setUsesLeftCount(period);
					m_thirdSkillCard->bind(roomSessionPlayer);
				}
			}
		}
	}
}

void PlayerSkillManager::initializeSkillGaugeBoosters()
{
	if (const auto player = m_player.lock(); player != nullptr)
	{
		switch (player->getPlayer()->getEquipmentManager()->getEquippedBooster())
		{
		case BOOSTER_CBOOST:
			m_skillGaugeBoostPercentage = 0.10;
			break;
		case BOOSTER_CBOOST_2:
			m_skillGaugeBoostPercentage = 0.50;
			break;
		default:
			m_skillGaugeBoostPercentage = 0.00;
			break;
		}
	}
}

void PlayerSkillManager::tick()
{
	if (hasActiveSkillCard() && m_activeSkillCard->hasDuration())
	{
		if (m_activeSkillCard->getDuration() > 0)
		{
			m_activeSkillCard->tick();
		}

		if (m_activeSkillCard->getDuration() == 0)
		{
			deactivateSkillCard();
		}
	}
}

void PlayerSkillManager::activateSkillCard(const std::shared_ptr<Skill>& skill, const uint32_t targetPlayerId,
	const uint64_t seqId, const uint32_t cardType)
{
	m_activeSkillCard = skill;

	m_activeSkillCardTargetPlayerId = targetPlayerId;
	m_activeSkillCardSeqId = seqId;

	m_cardType = cardType;

	if (const auto player = m_player.lock(); player != nullptr)
	{
		const auto roomSession = player->getRoomSession();

		if (roomSession == nullptr)
		{
			return;
		}

		const auto playerId = player->getPlayer()->getId();
		const auto itemId = m_activeSkillCard->getItemId();

		const auto skillTargetType = m_activeSkillCard->getSkillTargetType();

		if (const uint8_t team =
			(skillTargetType == SkillTargetType::ALLY_TEAM)
			? player->getTeam() :
			(skillTargetType == SkillTargetType::ENEMY_TEAM)
			? (player->getTeam() == 1 ? 2 : 1) : 0; team == 1 || team == 2)
		{
			for (const auto& roomSessionPlayer : roomSession->getPlayersForTeam(team))
			{
				m_skillTargetPlayerIds.push_back(roomSessionPlayer->getPlayer()->getId());
			}
		}

		skill->use();

		const auto usesLeftCount = skill->getUsesLeftCount();

		roomSession->relayPlaying<GCCard>(playerId, targetPlayerId, CGCard::ACTIVATE_CARD, cardType, itemId, seqId, usesLeftCount, m_skillTargetPlayerIds);

		removeSkillPoints(getRequiredSkillPoints(skill));

		player->getSkillManager()->getActiveSkillCard()->onApply();
	}
}

void PlayerSkillManager::deactivateSkillCard()
{
	if (m_activeSkillCard == nullptr)
	{
		return;
	}

	if (const auto player = m_player.lock(); player != nullptr)
	{
		const auto playerId = player->getPlayer()->getId();
		const auto itemId = m_activeSkillCard->getItemId();

		player->getRoomSession()->relayPlaying<GCCard>(playerId, m_activeSkillCardTargetPlayerId, CGCard::DEACTIVATE_CARD, m_cardType, itemId, m_activeSkillCardSeqId, m_skillTargetPlayerIds);
		player->getSkillManager()->getActiveSkillCard()->onWearOff();
	}

	//m_drawnSkillCard = nullptr;
	m_cardType = 0;
	m_activeSkillCard = nullptr;

	m_activeSkillCardTargetPlayerId = 0;
	m_activeSkillCardSeqId = 0;

	m_skillTargetPlayerIds.clear();

	updateSkillPointsForPlayer();
}

void PlayerSkillManager::failSkillCard(const std::shared_ptr<Skill>& skill, const uint32_t targetPlayerId,
	const uint64_t seqId, const uint32_t cardType)
{
	if (const auto player = m_player.lock(); player != nullptr)
	{
		const auto playerId = player->getPlayer()->getId();
		const auto itemId = skill->getItemId();

		skill->use();

		const auto usesLeftCount = skill->getUsesLeftCount();

		player->getRoomSession()->relayPlaying<GCCard>(playerId, targetPlayerId, CGCard::FAIL_CARD_ACTIVATION, cardType, itemId, seqId, usesLeftCount);

		removeSkillPoints(getRequiredSkillPoints(skill));

		if (usesLeftCount == 0)
		{
			//skill = nullptr;
		}
	}
}

bool PlayerSkillManager::isDrawnOrEquippedSkillCard(const uint32_t itemId) const
{
	const auto isDrawnSkillCard = (m_drawnSkillCard != nullptr) && (m_drawnSkillCard->getItemId() == itemId);
	const auto isEquippedSkillCard = (m_firstSkillCard != nullptr && m_firstSkillCard->getItemId() == itemId)
		|| (m_secondSkillCard != nullptr && m_secondSkillCard->getItemId() == itemId)
		|| (m_thirdSkillCard != nullptr && m_thirdSkillCard->getItemId() == itemId);

	return (isDrawnSkillCard || isEquippedSkillCard);
}

void PlayerSkillManager::updateSkillPointsForPlayer() const
{
	const auto guagePercentage = m_skillPoints % 100;
	const auto guagePoints = m_skillPoints / 100;

	if (const auto player = m_player.lock(); player != nullptr)
	{
		player->post(new GCCard(player->getPlayer()->getId(), guagePercentage, guagePoints));
	}
}

void PlayerSkillManager::addSkillPoints(const uint32_t skillPoints)
{
	m_skillPoints += skillPoints;

	updateSkillPointsForPlayer();
}

void PlayerSkillManager::removeSkillPoints(const uint32_t skillPoints)
{
	if (m_skillPoints <= skillPoints)
	{
		m_skillPoints = 0;
	}
	else
	{
		m_skillPoints -= skillPoints;
	}

	updateSkillPointsForPlayer();
}

void PlayerSkillManager::resetSkillPoints()
{
	m_skillPoints = 0;

	updateSkillPointsForPlayer();
}

uint32_t PlayerSkillManager::drawSkill()
{
	if (const auto player = m_player.lock(); player != nullptr)
	{
		std::unordered_map<uint32_t, uint32_t> skills;

		m_drawnSkillCard = player->getRoomSession()->getSkillManager()->generateRandomSkill();

		if (m_drawnSkillCard == nullptr)
		{
			return 0;
		}

		m_drawnSkillCard->bind(player);

		return m_drawnSkillCard->getItemId();
	}

	return 0;
}

std::shared_ptr<Skill> PlayerSkillManager::getActiveSkillCard() const
{
	return m_activeSkillCard;
}

std::shared_ptr<Skill> PlayerSkillManager::getDrawnSkillCard() const
{
	return m_drawnSkillCard;
}

std::shared_ptr<Skill> PlayerSkillManager::getEquippedSkillCard(const uint64_t seqId) const
{
	for (const auto& skillCard : { m_firstSkillCard, m_secondSkillCard, m_thirdSkillCard })
	{
		if (skillCard != nullptr && skillCard->getItemId() == seqId)
		{
			return skillCard;
		}
	}

	return nullptr;
}

std::array<std::shared_ptr<Skill>, 3> PlayerSkillManager::getEquippedSkillCards() const
{
	std::array<std::shared_ptr<Skill>, 3> skillCards = {};

	skillCards[0] = m_firstSkillCard;
	skillCards[1] = m_secondSkillCard;
	skillCards[2] = m_thirdSkillCard;

	return skillCards;
}

uint32_t PlayerSkillManager::getActiveSkillCardTargetPlayerId() const
{
	return m_activeSkillCardTargetPlayerId;
}

uint64_t PlayerSkillManager::getActiveSkillCardSeqId() const
{
	return m_activeSkillCardSeqId;
}

uint32_t PlayerSkillManager::getRequiredSkillPoints(const std::shared_ptr<Skill>& skill)
{
	return (skill != nullptr)
		? (skill->getSkillPointCost() * 100)
		: std::numeric_limits<unsigned int>::max();
}

double PlayerSkillManager::getSkillGaugeBoostPercentage() const
{
	return m_skillGaugeBoostPercentage;
}

bool PlayerSkillManager::hasSufficientSkillPoints(const std::shared_ptr<Skill>& skill) const
{
	return (m_skillPoints >= getRequiredSkillPoints(skill));
}

bool PlayerSkillManager::hasActiveSkillCard() const
{
	return  (m_activeSkillCard != nullptr);
}

bool PlayerSkillManager::isDrawnSkillCard(const uint32_t itemId) const
{
	return (m_drawnSkillCard->getItemId() == itemId);
}

std::vector<uint32_t> PlayerSkillManager::getSkillTargetPlayerIds() const
{
	return m_skillTargetPlayerIds;
}
