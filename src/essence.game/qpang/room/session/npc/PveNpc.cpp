#include "PveNpc.h"

#include "RoomSession.h"

PveNpc::PveNpc(const eNpcType type, const Position& initialSpawnPosition, const uint16_t baseHealth,
	const uint16_t initialSpawnRotation, const bool shouldRespawn) :
	m_type(type),
	m_position(initialSpawnPosition),
	m_initialSpawnPosition(initialSpawnPosition),
	m_health(baseHealth),
	m_baseHealth(baseHealth),
	m_initialSpawnRotation(initialSpawnRotation),
	m_shouldRespawn(shouldRespawn)
{
}

/*void PveNpc::tick(const std::shared_ptr<RoomSession>& roomSession)
{
}*/

void PveNpc::setUid(const uint32_t uid)
{
	m_uid = uid;
}

void PveNpc::resetHealth()
{
	m_health = m_baseHealth;
}

uint16_t PveNpc::takeDamage(const uint16_t damage)
{
	if (damage > m_health)
	{
		m_health = 0;

		return (damage - m_health);
	}

	m_health = m_health - damage;

	return damage;
}

uint32_t PveNpc::getUid() const
{
	return m_uid;
}

eNpcType PveNpc::getType() const
{
	return m_type;
}

Position PveNpc::getPosition() const
{
	return m_position;
}

Position PveNpc::getInitialSpawnPosition() const
{
	return m_initialSpawnPosition;
}

uint16_t PveNpc::getHealth() const
{
	return m_health;
}

uint16_t PveNpc::getBaseHealth() const
{
	return m_baseHealth;
}

uint16_t PveNpc::getInitialSpawnRotation() const
{
	return m_initialSpawnRotation;
}

bool PveNpc::shouldRespawn() const
{
	return m_shouldRespawn;
}

bool PveNpc::isDead() const
{
	return m_health <= 0;
}
