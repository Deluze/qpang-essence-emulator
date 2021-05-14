#include "PlayerBaseEntity.h"
#include <iostream>


PlayerBaseEntity::PlayerBaseEntity(uint32_t id) : m_id(id), m_killCount(0)
{
}

uint32_t PlayerBaseEntity::getId()
{
	return m_id;
}

uint8_t PlayerBaseEntity::getKillCount()
{
	return m_killCount;
}

bool PlayerBaseEntity::isPlayerValidForHit(uint32_t playerId)
{
	return std::find(m_hitPlayers.cbegin(), m_hitPlayers.cend(), playerId) != m_hitPlayers.cend();
}

void PlayerBaseEntity::addKill()
{
	m_killCount++;
}

void PlayerBaseEntity::onPlayerHit(uint32_t playerId)
{
	m_hitPlayers.push_back(playerId);
}