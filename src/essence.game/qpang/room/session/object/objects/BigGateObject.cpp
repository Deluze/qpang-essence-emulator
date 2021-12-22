#include "BigGateObject.h"

#include "RoomSession.h"

BigGateObject::BigGateObject(const eBigGateActionId actionId, const Position& startPos, const Position& endPos) : PveObject(eObjectType::BIG_GATE, startPos)
{
	m_endPosition = endPos;
	m_actionId = actionId;
}

void BigGateObject::tick(const std::shared_ptr<RoomSession>& roomSession)
{
	m_shouldMove = true;

	move(roomSession, m_position, m_endPosition, 1);
}