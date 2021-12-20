#include "PveObject.h"

#include "RoomSession.h"

PveObject::PveObject(eObjectType type, const Position& position)
{
	m_uid = 0;
	m_type = type;
	m_position = position;
}

eObjectType PveObject::getType()
{
	return m_type;
}

Position PveObject::getPosition()
{
	return m_position;
}

void PveObject::setUid(uint32_t uid)
{
	m_uid = uid;
}

void PveObject::tick(std::shared_ptr<RoomSession> roomSession)
{

}