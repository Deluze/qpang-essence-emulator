#include "PveItem.h"

PveItem::PveItem(const eItemType type, const Position& position) :
	m_type(type),
	m_position(position)
{
}

void PveItem::tick(const std::shared_ptr<RoomSession>& roomSession)
{

}

void PveItem::setUid(const uint32_t uid)
{
	m_uid = uid;
}

uint32_t PveItem::getUid() const
{
	return m_uid;
}

eItemType PveItem::getType() const
{
	return m_type;
}

Position PveItem::getPosition() const
{
	return m_position;
}
