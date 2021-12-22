#include "PveObject.h"

#include "RoomSession.h"

#include <qpang/room/tnl/net_events/server/gc_pve_object_init.hpp>
#include <qpang/room/tnl/net_events/server/gc_pve_object_move.hpp>

PveObject::PveObject(const eObjectType type, const Position& position)
{
	m_shouldMove = false;
	m_moveTickCount = 0;
	m_uid = 0;
	m_type = type;
	m_position = position;
}

eObjectType PveObject::getType() const
{
	return m_type;
}

Position PveObject::getPosition() const
{
	return m_position;
}

void PveObject::setUid(const uint32_t uid)
{
	m_uid = uid;
}

void PveObject::setShouldMove(const bool shouldMove)
{
	m_shouldMove = shouldMove;
}

void PveObject::move(const std::shared_ptr<RoomSession>& roomSession, const Position& from, const Position& to, int ticks, int waitTicks)
{
	// Using inits to minimize the chance of desyncs between players

	if (!m_shouldMove)
		return;

	if (m_moveTickCount == 0)
		roomSession->relayPlaying<GCPvEObjectMove>(m_uid, to.x, to.y, to.z, ticks * 1000);

	if (m_moveTickCount == ticks)
		roomSession->relayPlaying<GCPvEObjectInit>(static_cast<uint32_t>(m_type), m_uid, to.x, to.y, to.z, 0);

	if (m_moveTickCount == ticks + waitTicks)
		roomSession->relayPlaying<GCPvEObjectMove>(m_uid, from.x, from.y, from.z, ticks * 1000);

	if (m_moveTickCount == ticks * 2 + waitTicks)
		roomSession->relayPlaying<GCPvEObjectInit>(static_cast<uint32_t>(m_type), m_uid, from.x, from.y, from.z, 0);

	if (waitTicks == 0)
	{
		if (m_moveTickCount == ticks * 2 + waitTicks * 2)
		{
			// Start moving it already, set tickcount to 0 as it will be 1 next tick, because the object has already been moving for 1 tick then.
			// We can't just set the tickcount to -1 now, because it will take 1 tick from now before the next tick is called, effectively
			// adding 1 tick of wait time, that we don't want.
			roomSession->relayPlaying<GCPvEObjectMove>(m_uid, to.x, to.y, to.z, ticks * 1000);
			m_moveTickCount = 0;
		}
	}
	else
	{
		// -1 because as explained above, it will take 1 tick before the next tick is called and the m_moveTickCount == 0 check will be hit
		if (m_moveTickCount == ticks * 2 + waitTicks * 2 - 1)
			m_moveTickCount = -1;
	}

	m_moveTickCount++;
}

void PveObject::tick(const std::shared_ptr<RoomSession>& roomSession)
{

}

void PveObject::onEvent(const std::shared_ptr<RoomSession>& roomSession)
{

}