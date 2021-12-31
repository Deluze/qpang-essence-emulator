#include "PveObject.h"

#include "RoomSession.h"
#include "gc_pve_hit_npc_to_object.hpp"

PveObject::PveObject(const uint32_t uid, const eObjectType type, const Position& position,
                     const uint32_t linkedObjectId, const uint16_t initialHealth) :
	m_uid(uid),
	m_linkedObjectId(linkedObjectId),
	m_type(type),
	m_position(position),
	m_initialHealth(initialHealth),
	m_health(initialHealth)
{
}

void PveObject::tick(const std::shared_ptr<RoomSession>& roomSession)
{

}

void PveObject::takeDamage(const uint16_t damage)
{
	if (damage > m_health)
	{
		m_health = 0;

		return;
	}

	m_health = m_health - damage;
}

void PveObject::onHitByNpc(const std::shared_ptr<RoomSession>& roomSession)
{
	// TODO: Deal 10/15/25 damage based on weapon type.. if we can even get that.
	constexpr auto damage = 10;

	takeDamage(damage);

	// TODO: Finish the game if health is 0.

	roomSession->relayPlaying<GCPvEHitNpcToObject>(m_uid, m_health, damage);
}

void PveObject::onEvent(const std::shared_ptr<RoomSession>& roomSession)
{
	// Default is 0, not all objects have a linked object
	// TODO: would be more proper to check if the linked object is a gate, before opening
	if (m_linkedObjectId)
	{
		roomSession->getObjectManager()->openGate(m_linkedObjectId);
	}
}

eObjectType PveObject::getType() const
{
	return m_type;
}

Position PveObject::getPosition() const
{
	return m_position;
}

uint16_t PveObject::getInitialHealth() const
{
	return m_initialHealth;
}

uint16_t PveObject::getHealth() const
{
	return m_health;
}