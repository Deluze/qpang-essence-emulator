#include "RedBoardObject.h"

#include "RoomSession.h"

#include <qpang/room/tnl/net_events/server/gc_pve_object_move.hpp>
#include <qpang/room/tnl/net_events/server/gc_pve_object_init.hpp>

RedBoardObject::RedBoardObject(eRedBoardActionId actionId, const Position& position) : PveObject(eObjectType::MOVING_RED_BOARD, position)
{
	m_actionId = actionId;
}

void RedBoardObject::tick(std::shared_ptr<RoomSession> roomSession)
{
	if (m_actionId == eRedBoardActionId::STAGE0_PIT1_BOARD1)
	{
		if (m_tickCount == 0)
			roomSession->relayPlaying<GCPvEObjectInit>((U32)m_type, m_uid, -13.64f, -0.5f, -22.64f, 0);
		else if (m_tickCount == 1)
			roomSession->relayPlaying<GCPvEObjectMove>(m_uid, -1.4f, -0.5f, -22.64f, 7000);
		else if (m_tickCount == 8)
			roomSession->relayPlaying<GCPvEObjectInit>((U32)m_type, m_uid, -1.4f, -0.5f, -22.64f, 0);
		else if (m_tickCount == 9)
			roomSession->relayPlaying<GCPvEObjectMove>(m_uid, -13.64f, -0.5f, -22.64f, 7000);
		else if (m_tickCount == 15)
			m_tickCount = -1;
	}

	m_tickCount++;
}