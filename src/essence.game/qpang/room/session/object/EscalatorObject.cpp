#include "EscalatorObject.h"

#include "RoomSession.h"

#include <qpang/room/tnl/net_events/server/gc_pve_object_move.hpp>
#include <qpang/room/tnl/net_events/server/gc_pve_object_init.hpp>

EscalatorObject::EscalatorObject(eRedBoardActionId actionId, const Position& position) : PveObject(eObjectType::MOVING_ESCALATOR, position)
{
	m_actionId = actionId;
}

void EscalatorObject::move(std::shared_ptr<RoomSession> roomSession, const Position& from, const Position& to, int ticks)
{
	// Using inits to minimize desyncs between players
	if (m_tickCount == 0)
		roomSession->relayPlaying<GCPvEObjectInit>((U32)m_type, m_uid, from.x, from.y, from.z, 0);
	else if (m_tickCount == 1)
		roomSession->relayPlaying<GCPvEObjectMove>(m_uid, to.x, to.y, to.z, ticks * 1000);
	else if (m_tickCount == ticks + 1)
		roomSession->relayPlaying<GCPvEObjectInit>((U32)m_type, m_uid, to.x, to.y, to.z, 0);
	else if (m_tickCount == ticks + 2)
		roomSession->relayPlaying<GCPvEObjectMove>(m_uid, from.x, from.y, from.z, ticks * 1000);
	else if (m_tickCount == 2 * ticks + 1)
		m_tickCount = -1;

	m_tickCount++;
}

void EscalatorObject::tick(std::shared_ptr<RoomSession> roomSession)
{
	if (m_actionId == eRedBoardActionId::STAGE0_PIT1_ESCALATOR1)
		move(roomSession, m_position, { -1.4f, -0.5f, -22.64f }, 5);
	else if (m_actionId == eRedBoardActionId::STAGE0_PIT2_ESCALATOR1)
		move(roomSession, m_position, { 36.56f, -0.5f, -23.45f }, 8);

	
}