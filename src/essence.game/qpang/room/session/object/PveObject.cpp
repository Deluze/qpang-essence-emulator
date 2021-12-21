#include "PveObject.h"

#include "RoomSession.h"

#include <qpang/room/tnl/net_events/server/gc_pve_object_init.hpp>
#include <qpang/room/tnl/net_events/server/gc_pve_object_move.hpp>

PveObject::PveObject(eObjectType type, const Position& position)
{
	m_moveTickCount = 0;
	m_uid = 0;
	m_type = type;
	m_position = position;
}

eObjectType PveObject::getType()
{
	return m_type;
}

Position PveObject::getPosition()
{
	return m_position;
}

void PveObject::setUid(uint32_t uid)
{
	m_uid = uid;
}

void PveObject::move(std::shared_ptr<RoomSession> roomSession, const Position& from, const Position& to, int ticks)
{
	// Using inits to minimize desyncs between players
	if (m_moveTickCount == 0)
		roomSession->relayPlaying<GCPvEObjectInit>((U32)m_type, m_uid, from.x, from.y, from.z, 0);
	else if (m_moveTickCount == 1)
		roomSession->relayPlaying<GCPvEObjectMove>(m_uid, to.x, to.y, to.z, ticks * 1000);
	else if (m_moveTickCount == ticks + 1)
		roomSession->relayPlaying<GCPvEObjectInit>((U32)m_type, m_uid, to.x, to.y, to.z, 0);
	else if (m_moveTickCount == ticks + 2)
		roomSession->relayPlaying<GCPvEObjectMove>(m_uid, from.x, from.y, from.z, ticks * 1000);
	else if (m_moveTickCount == 2 * ticks + 1)
		m_moveTickCount = -1;

	m_moveTickCount++;
}

void PveObject::tick(std::shared_ptr<RoomSession> roomSession)
{

}