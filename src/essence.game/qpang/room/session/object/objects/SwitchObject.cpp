#include "SwitchObject.h"

#include "RoomSession.h"

SwitchObject::SwitchObject(uint32_t linkedGateId, const Position& position) : PveObject(eObjectType::SWITCH, position)
{
	m_linkedGateId = linkedGateId;
}

void SwitchObject::tick(std::shared_ptr<RoomSession> roomSession)
{
	
}

void SwitchObject::onEvent(std::shared_ptr<RoomSession> roomSession)
{
	auto objectPtr = roomSession->getObjectManager()->findObjectByUid(m_linkedGateId);
	if (!objectPtr)
		return;

	objectPtr->setShouldMove(true);
}