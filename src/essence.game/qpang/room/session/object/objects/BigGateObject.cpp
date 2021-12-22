#include "BigGateObject.h"

#include "RoomSession.h"

BigGateObject::BigGateObject(const Position& position) : PveObject(eObjectType::BIG_GATE, position) {}

void BigGateObject::tick(const std::shared_ptr<RoomSession>& roomSession) {}