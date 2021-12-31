#include "PveObject.h"

#include "RoomSession.h"

#include <qpang/room/tnl/net_events/server/gc_pve_object_move.hpp>

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

void PveObject::tick(const std::shared_ptr<RoomSession>& roomSession)
{

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