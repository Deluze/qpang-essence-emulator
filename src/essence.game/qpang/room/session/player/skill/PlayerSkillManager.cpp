#include "PlayerSkillManager.h"

#include "qpang/Game.h"

#include "qpang/player/Player.h"
#include "qpang/room/session/player/RoomSessionPlayer.h"
#include "qpang/player/equipment/EquipmentManager.h"

#include "qpang/room/tnl/net_events/server/gc_card.hpp"

void PlayerSkillManager::initialize(const std::shared_ptr<RoomSessionPlayer>& player)
{
	m_player = player;

	if (const auto roomSessionPlayer = m_player.lock(); player != nullptr)
	{
		switch (roomSessionPlayer->getPlayer()->getEquipmentManager()->getEquippedBooster())
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

	resetSkillPoints();
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

void PlayerSkillManager::activateSkillCard(const uint32_t targetPlayerId, const uint64_t seqId)
{
	if (!hasSufficientSkillPoints())
	{
		failSkillCard(targetPlayerId, seqId);

		return;
	}

	m_activeSkillCard = m_drawnSkillCard;

	m_activeSkillCardTargetPlayerId = targetPlayerId;
	m_activeSkillCardSeqId = seqId;

	if (const auto player = m_player.lock(); player != nullptr)
	{
		const auto roomSession = player->getRoomSession();

		if (roomSession == nullptr)
		{
			return;
		}

		const auto playerId = player->getPlayer()->getId();
		const auto itemId = m_activeSkillCard->getItemId();

		roomSession->relayPlaying<GCCard>(playerId, m_activeSkillCardTargetPlayerId, CGCard::ACTIVATE_CARD, CGCard::SKILL_CARD, itemId, m_activeSkillCardSeqId);

		removeSkillPoints(getRequiredSkillPoints());

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

		player->getRoomSession()->relayPlaying<GCCard>(playerId, m_activeSkillCardTargetPlayerId, CGCard::DEACTIVATE_CARD, CGCard::SKILL_CARD, itemId, m_activeSkillCardSeqId);

		player->getSkillManager()->getActiveSkillCard()->onWearOff();
	}

	m_drawnSkillCard = nullptr;
	m_activeSkillCard = nullptr;

	m_activeSkillCardTargetPlayerId = 0;
	m_activeSkillCardSeqId = 0;

	updateSkillPointsForPlayer();
}

void PlayerSkillManager::failSkillCard(const uint32_t targetPlayerId, const uint64_t seqId)
{
	if (const auto player = m_player.lock(); player != nullptr)
	{
		const auto playerId = player->getPlayer()->getId();
		const auto itemId = m_drawnSkillCard->getItemId();

		player->getRoomSession()->relayPlaying<GCCard>(playerId, targetPlayerId, CGCard::FAIL_CARD_ACTIVATION, CGCard::SKILL_CARD, itemId, seqId);

		const auto requiredSkillPoints = getRequiredSkillPoints();

		removeSkillPoints(requiredSkillPoints);

		m_drawnSkillCard = nullptr;
	}
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


uint32_t PlayerSkillManager::getActiveSkillCardTargetPlayerId() const
{
	return m_activeSkillCardTargetPlayerId;
}

uint64_t PlayerSkillManager::getActiveSkillCardSeqId() const
{
	return m_activeSkillCardSeqId;
}

uint32_t PlayerSkillManager::getRequiredSkillPoints() const
{
	if (m_drawnSkillCard == nullptr)
	{
		return std::numeric_limits<unsigned int>::max();
	}

	return (m_drawnSkillCard->getSkillPointCost() * 100);
}


double PlayerSkillManager::getSkillGaugeBoostPercentage() const
{
	return m_skillGaugeBoostPercentage;
}

bool PlayerSkillManager::hasSufficientSkillPoints() const
{
	const auto requiredSkillPoints = getRequiredSkillPoints();

	return (m_skillPoints >= requiredSkillPoints);
}

bool PlayerSkillManager::hasActiveSkillCard() const
{
	return  (m_activeSkillCard != nullptr);
}

bool PlayerSkillManager::isDrawnSkillCard(const uint32_t itemId) const
{
	return (m_drawnSkillCard->getItemId() == itemId);
}