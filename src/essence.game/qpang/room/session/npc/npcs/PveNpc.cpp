#include "PveNpc.h"

PveNpc::PveNpc(const eNpcType type, const Position& position) :
	m_type(type),
	m_position(position)
{
}

void PveNpc::tick(const std::shared_ptr<RoomSession>& roomSession)
{

}

void PveNpc::setUid(const uint32_t uid)
{
	m_uid = uid;
}

uint32_t PveNpc::getUid() const
{
	return m_uid;
}

eNpcType PveNpc::getType() const
{
	return m_type;
}

Position PveNpc::getPosition() const
{
	return m_position;
}
