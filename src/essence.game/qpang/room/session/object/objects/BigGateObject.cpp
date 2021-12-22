#include "BigGateObject.h"

#include "RoomSession.h"

BigGateObject::BigGateObject(const Position& startPos) : PveObject(eObjectType::BIG_GATE, startPos) {}

void BigGateObject::tick(const std::shared_ptr<RoomSession>& roomSession) {}