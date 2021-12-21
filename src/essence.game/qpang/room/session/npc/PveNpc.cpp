#include "PveNpc.h"

#include <iostream>

PveNpc::PveNpc(const eNpcType type, const Position& position, const uint16_t baseHealth) :
	m_type(type),
	m_position(position),
	m_health(baseHealth)
{
}

/*void PveNpc::tick(const std::shared_ptr<RoomSession>& roomSession)
{

}*/

void PveNpc::setUid(const uint32_t uid)
{
	m_uid = uid;
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

uint16_t PveNpc::getHealth() const
{
	return m_health;
}

bool PveNpc::isDead() const
{
	return m_health <= 0;
}
