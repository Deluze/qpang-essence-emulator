#include "SwitchObject.h"

#include <qpang/room/tnl/net_events/server/gc_pve_door.hpp>

#include "RoomSession.h"

SwitchObject::SwitchObject(uint32_t linkedGateId, const Position& position) : PveObject(eObjectType::SWITCH, position),
	m_linkedGateId(linkedGateId)
{
}

void SwitchObject::tick(const std::shared_ptr<RoomSession>& roomSession)
{
	
}

void SwitchObject::onEvent(const std::shared_ptr<RoomSession>& roomSession)
{
	roomSession->getObjectManager()->openGate(m_linkedGateId);
}
