#include "EscalatorObject.h"

#include "RoomSession.h"

#include <qpang/room/tnl/net_events/server/gc_pve_object_move.hpp>
#include <qpang/room/tnl/net_events/server/gc_pve_object_init.hpp>

EscalatorObject::EscalatorObject(eRedBoardActionId actionId, const Position& position) : PveObject(eObjectType::MOVING_ESCALATOR, position)
{
	m_actionId = actionId;
}

void EscalatorObject::tick(std::shared_ptr<RoomSession> roomSession)
{
	if (m_actionId == eRedBoardActionId::STAGE0_PIT1_ESCALATOR1)
		move(roomSession, m_position, { -1.4f, -0.5f, -22.64f }, 5);
	else if (m_actionId == eRedBoardActionId::STAGE0_PIT2_ESCALATOR1)
		move(roomSession, m_position, { 36.56f, -0.5f, -23.45f }, 8);

	
}