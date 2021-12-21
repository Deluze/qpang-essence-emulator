#include "BigGateObject.h"

#include "RoomSession.h"

BigGateObject::BigGateObject(eBigGateActionId actionId, const Position& position) : PveObject(eObjectType::BIG_GATE, position)
{
	m_actionId = actionId;
}

void BigGateObject::tick(std::shared_ptr<RoomSession> roomSession)
{
	// to:
	// 39.1, 2, -5.1
}