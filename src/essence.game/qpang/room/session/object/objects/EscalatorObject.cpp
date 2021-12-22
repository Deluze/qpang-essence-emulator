#include "EscalatorObject.h"

#include "RoomSession.h"

EscalatorObject::EscalatorObject(const EscalatorMoveInfo& moveInfo) : PveObject(eObjectType::MOVING_ESCALATOR, moveInfo.m_startPos)
{
	m_shouldMove = true;
	m_moveInfo = moveInfo;
}

void EscalatorObject::tick(const std::shared_ptr<RoomSession>& roomSession)
{
	move(roomSession, m_moveInfo.m_startPos, m_moveInfo.m_endPos, m_moveInfo.m_durationSeconds, m_moveInfo.m_waitSeconds);
}