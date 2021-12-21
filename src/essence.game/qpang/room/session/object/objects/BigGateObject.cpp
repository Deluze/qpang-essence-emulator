#include "BigGateObject.h"

#include "RoomSession.h"

BigGateObject::BigGateObject(eBigGateActionId actionId, const Position& startPos, const Position& endPos) : PveObject(eObjectType::BIG_GATE, startPos)
{
	m_endPosition = endPos;
	m_actionId = actionId;
}

void BigGateObject::tick(std::shared_ptr<RoomSession> roomSession)
{
	m_shouldMove = true;
	move(roomSession, m_position, m_endPosition, 1);
}