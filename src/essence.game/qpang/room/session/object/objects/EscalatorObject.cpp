#include "EscalatorObject.h"

#include "RoomSession.h"

EscalatorObject::EscalatorObject(const eEscalatorActionId actionId, const Position& position) : PveObject(eObjectType::MOVING_ESCALATOR, position)
{
	m_shouldMove = true;
	m_actionId = actionId;
}

void EscalatorObject::tick(const std::shared_ptr<RoomSession>& roomSession)
{
	if (m_actionId == eEscalatorActionId::STAGE0_PIT1_ESCALATOR1)
		move(roomSession, m_position, { -1.4f, -0.5f, -22.64f }, 5);
	else if (m_actionId == eEscalatorActionId::STAGE0_PIT2_ESCALATOR1)
		move(roomSession, m_position, { 36.56f, -0.5f, -23.45f }, 8);
}