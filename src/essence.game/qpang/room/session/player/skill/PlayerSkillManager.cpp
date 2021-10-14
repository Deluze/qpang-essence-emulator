#include "PlayerSkillManager.h"

#include "qpang/Game.h"

#include "qpang/player/Player.h"
#include "qpang/room/session/player/RoomSessionPlayer.h"
#include "qpang/player/equipment/EquipmentManager.h"

#include "qpang/room/tnl/net_events/server/gc_card.hpp"

void PlayerSkillManager::initialize(std::shared_ptr<RoomSessionPlayer> player)
{
	m_player = player;

	resetSkillPoints();
}

void PlayerSkillManager::tick()
{
	if (isSkillCardActive() && m_activeSkillCard->hasDuration())
	{
		if (m_activeSkillCard->getDurationInSeconds() > 0)
		{
			m_activeSkillCard->tick();
		}

		if (m_activeSkillCard->getDurationInSeconds() == 0)
		{
			deactivateSkillCard();
		}
	}
}

void PlayerSkillManager::activateSkillCard(uint32_t targetPlayerId, uint32_t seqId)
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

		player->getSkillManager()->getActiveSkillCard()->apply();

		roomSession->relayPlaying<GCCard>(playerId, m_activeSkillCardTargetPlayerId, CGCard::CARD_BEGIN, CGCard::SKILL_CARD, itemId, m_activeSkillCardSeqId);

		const auto requiredSkillPoints = getRequiredSkillPoints();

		removeSkillPoints(requiredSkillPoints);
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

		player->getRoomSession()->relayPlaying<GCCard>(playerId, m_activeSkillCardTargetPlayerId, CGCard::CARD_END, CGCard::SKILL_CARD, itemId, m_activeSkillCardSeqId);
	}

	m_drawnSkillCard = nullptr;
	m_activeSkillCard = nullptr;

	m_activeSkillCardTargetPlayerId = 0;
	m_activeSkillCardSeqId = 0;

	updateSkillPointsForPlayer();
}

void PlayerSkillManager::failSkillCard(uint32_t targetPlayerId, uint32_t seqId)
{
	if (const auto player = m_player.lock(); player != nullptr)
	{
		const auto playerId = player->getPlayer()->getId();
		const auto itemId = m_drawnSkillCard->getItemId();

		player->getRoomSession()->relayPlaying<GCCard>(playerId, targetPlayerId, CGCard::CARD_USE_FAIL, CGCard::SKILL_CARD, itemId, seqId);

		const auto requiredSkillPoints = getRequiredSkillPoints();

		removeSkillPoints(requiredSkillPoints);

		m_drawnSkillCard = nullptr;
	}
}

void PlayerSkillManager::updateSkillPointsForPlayer()
{
	const auto guagePercentage = m_skillPoints % 100;
	const auto guagePoints = m_skillPoints / 100;

	if (auto player = m_player.lock(); player != nullptr)
	{
		player->post(new GCCard(player->getPlayer()->getId(), guagePercentage, guagePoints));
	}
}

void PlayerSkillManager::addSkillPoints(uint32_t skillPoints)
{
	m_skillPoints += skillPoints;

	updateSkillPointsForPlayer();
}

void PlayerSkillManager::removeSkillPoints(uint32_t skillPoints)
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
	if (auto player = m_player.lock(); player != nullptr)
	{
		std::unordered_map<uint32_t, uint32_t> skills;

		m_drawnSkillCard = player->getRoomSession()->getSkillManager()->generateRandomSkill();
		//m_drawnSkillCard->bind(player);

		return m_drawnSkillCard->getItemId();
	}

	return 0;
}

std::shared_ptr<Skill> PlayerSkillManager::getActiveSkillCard()
{
	return m_activeSkillCard;
}

std::shared_ptr<Skill> PlayerSkillManager::getDrawnSkillCard()
{
	return m_drawnSkillCard;
}

uint32_t PlayerSkillManager::getRequiredSkillPoints()
{
	if (m_drawnSkillCard == nullptr)
	{
		return std::numeric_limits<unsigned int>::max();
	}

	return (m_drawnSkillCard->getRequiredSkillPoints() * 100);
}

bool PlayerSkillManager::hasSufficientSkillPoints()
{
	const auto requiredSkillPoints = getRequiredSkillPoints();

	return (m_skillPoints >= requiredSkillPoints);
}

bool PlayerSkillManager::isSkillCardActive()
{
	return (m_activeSkillCard != nullptr);
}

bool PlayerSkillManager::isDrawnSkillCard(uint32_t itemId)
{
	return (m_drawnSkillCard->getItemId() == itemId);
}