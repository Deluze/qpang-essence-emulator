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
				m_firstSkill = roomSessionPlayer->getRoomSession()->getSkillManager()->getSkillByItemId(itemId);

				if (m_firstSkill != nullptr)
				{
					m_firstSkill->setUsesLeftCount(period);
					m_firstSkill->bind(roomSessionPlayer);
				}

			}
			else if (i == 1)
			{
				m_secondSkill = roomSessionPlayer->getRoomSession()->getSkillManager()->getSkillByItemId(itemId);

				if (m_secondSkill != nullptr)
				{
					m_secondSkill->setUsesLeftCount(period);
					m_secondSkill->bind(roomSessionPlayer);
				}

			}
			else if (i == 2)
			{
				m_thirdSkill = roomSessionPlayer->getRoomSession()->getSkillManager()->getSkillByItemId(itemId);

				if (m_thirdSkill != nullptr)
				{
					m_thirdSkill->setUsesLeftCount(period);
					m_thirdSkill->bind(roomSessionPlayer);
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
	if (hasActiveSkill() && m_activeSkill->hasDuration())
	{
		if (m_activeSkill->getDuration() > 0)
		{
			m_activeSkill->tick();
		}

		if (m_activeSkill->getDuration() == 0)
		{
			deactivateSkill();
		}
	}
}

void PlayerSkillManager::activateSkill(const std::shared_ptr<Skill>& skill, const uint32_t targetPlayerId,
	const uint64_t seqId, const uint32_t cardType)
{
	m_activeSkill = skill;

	m_activeSkillTargetPlayerId = targetPlayerId;
	m_activeSkillSeqId = seqId;

	m_activeSkillCardType = cardType;

	if (const auto player = m_player.lock(); player != nullptr)
	{
		const auto roomSession = player->getRoomSession();

		if (roomSession == nullptr)
		{
			return;
		}

		const auto playerId = player->getPlayer()->getId();
		const auto itemId = m_activeSkill->getItemId();

		const auto skillTargetType = m_activeSkill->getSkillTargetType();

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

		player->getSkillManager()->getActiveSkill()->onApply();
	}
}

void PlayerSkillManager::deactivateSkill()
{
	if (m_activeSkill == nullptr)
	{
		return;
	}

	if (const auto player = m_player.lock(); player != nullptr)
	{
		const auto playerId = player->getPlayer()->getId();
		const auto itemId = m_activeSkill->getItemId();

		player->getRoomSession()->relayPlaying<GCCard>(playerId, m_activeSkillTargetPlayerId, CGCard::DEACTIVATE_CARD, m_activeSkillCardType, itemId, m_activeSkillSeqId, m_skillTargetPlayerIds);
		player->getSkillManager()->getActiveSkill()->onWearOff();
	}

	m_activeSkillCardType = 0;
	m_activeSkill = nullptr;

	m_activeSkillTargetPlayerId = 0;
	m_activeSkillSeqId = 0;

	m_skillTargetPlayerIds.clear();

	updateSkillPointsForPlayer();
}

void PlayerSkillManager::failSkill(const std::shared_ptr<Skill>& skill, const uint32_t targetPlayerId,
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

bool PlayerSkillManager::isDrawnOrEquippedSkill(const uint32_t itemId) const
{
	const auto isDrawnSkillCard = (m_drawnSkill != nullptr) && (m_drawnSkill->getItemId() == itemId);
	const auto isEquippedSkillCard = (m_firstSkill != nullptr && m_firstSkill->getItemId() == itemId)
		|| (m_secondSkill != nullptr && m_secondSkill->getItemId() == itemId)
		|| (m_thirdSkill != nullptr && m_thirdSkill->getItemId() == itemId);

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

		m_drawnSkill = player->getRoomSession()->getSkillManager()->generateRandomSkill();

		if (m_drawnSkill == nullptr)
		{
			return 0;
		}

		m_drawnSkill->bind(player);

		return m_drawnSkill->getItemId();
	}

	return 0;
}

std::shared_ptr<Skill> PlayerSkillManager::getActiveSkill() const
{
	return m_activeSkill;
}

std::shared_ptr<Skill> PlayerSkillManager::getDrawnSkill() const
{
	return m_drawnSkill;
}

std::shared_ptr<Skill> PlayerSkillManager::getEquippedSkill(const uint64_t seqId) const
{
	for (const auto& skillCard : { m_firstSkill, m_secondSkill, m_thirdSkill })
	{
		if (skillCard != nullptr && skillCard->getItemId() == seqId)
		{
			return skillCard;
		}
	}

	return nullptr;
}

std::array<std::shared_ptr<Skill>, 3> PlayerSkillManager::getEquippedSkills() const
{
	std::array<std::shared_ptr<Skill>, 3> skillCards = {};

	skillCards[0] = m_firstSkill;
	skillCards[1] = m_secondSkill;
	skillCards[2] = m_thirdSkill;

	return skillCards;
}

uint32_t PlayerSkillManager::getActiveSkillTargetPlayerId() const
{
	return m_activeSkillTargetPlayerId;
}

uint64_t PlayerSkillManager::getActiveSkillSeqId() const
{
	return m_activeSkillSeqId;
}

uint32_t PlayerSkillManager::getActiveSkillCardType()
{
	return m_activeSkillCardType;
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

bool PlayerSkillManager::hasActiveSkill() const
{
	return  (m_activeSkill != nullptr);
}

bool PlayerSkillManager::isDrawnSkill(const uint32_t itemId) const
{
	return (m_drawnSkill->getItemId() == itemId);
}

std::vector<uint32_t> PlayerSkillManager::getSkillTargetPlayerIds() const
{
	return m_skillTargetPlayerIds;
}
