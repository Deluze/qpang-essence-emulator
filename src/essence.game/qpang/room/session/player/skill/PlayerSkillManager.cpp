#include "PlayerSkillManager.h"

#include "qpang/Game.h"

#include "qpang/player/Player.h"
#include "qpang/room/session/player/RoomSessionPlayer.h"
#include "qpang/player/equipment/EquipmentManager.h"

#include "qpang/room/tnl/net_events/server/gc_card.hpp"

void PlayerSkillManager::initialize(std::shared_ptr<RoomSessionPlayer> player)
{
	m_player = player;
}

void PlayerSkillManager::tick()
{
}

void PlayerSkillManager::removeSkillPoints(uint32_t amount)
{
	if (m_skillPoints <= amount)
		m_skillPoints = 0;
	else
		m_skillPoints = amount;

	if (auto player = m_player.lock(); player != nullptr)
		player->post(new GCCard(player->getPlayer()->getId(), m_skillPoints % 100, m_skillPoints / 100));
}

void PlayerSkillManager::addSkillPoints(uint32_t amount)
{
	m_skillPoints += amount;

	if (auto player = m_player.lock(); player != nullptr)
		player->post(new GCCard(player->getPlayer()->getId(), m_skillPoints % 100, m_skillPoints / 100));
}

void PlayerSkillManager::resetPoints()
{
	m_skillPoints = 0;

	if (auto player = m_player.lock(); player != nullptr)
		player->post(new GCCard(player->getPlayer()->getId(), 0, 0));
}

uint32_t PlayerSkillManager::drawSkill()
{
	auto player = m_player.lock();

	if (player == nullptr)
		return 0;


	// TODO: Draw skill, return item id of skill.
	//m_drawnSkillCard = player->getRoomSession()->getSkillManager()->generateRandomSkill();
	//m_drawnSkillCard->bind(player);

	//return m_drawnSkillCard->getId();
	return 0;
}