#include "SwitchObject.h"

#include "gc_pve_event_object.hpp"
#include "RoomSession.h"

SwitchObject::SwitchObject(const uint32_t linkedGateId, const Position& position) : PveObject(eObjectType::SWITCH, position),
	m_linkedGateId(linkedGateId)
{
}

void SwitchObject::tick(const std::shared_ptr<RoomSession>& roomSession)
{
	
}

void SwitchObject::onEvent(const std::shared_ptr<RoomSession>& roomSession)
{
	const auto objectPtr = roomSession->getObjectManager()->findObjectByUid(m_linkedGateId);

	if (objectPtr == nullptr)
	{
		return;
	}

	objectPtr->setShouldMove(true);

	roomSession->getObjectManager()->openGate(m_linkedGateId);
}
