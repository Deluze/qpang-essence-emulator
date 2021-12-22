#include "SmallGateObject.h"

#include "RoomSession.h"

SmallGateObject::SmallGateObject(const Position& position) : PveObject(eObjectType::SMALL_GATE, position) {}

void SmallGateObject::tick(const std::shared_ptr<RoomSession>& roomSession) {}