#include "PveItem.h"

#include "gc_pve_item_init.hpp"

PveItem::PveItem(const uint32_t itemId, const Position& position) :
	m_itemId(itemId),
	m_position(position)
{
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void PveItem::tick(const std::shared_ptr<RoomSession>& roomSession)
{

}

void PveItem::spawn(const std::shared_ptr<RoomSession>& roomSession) const
{
	roomSession->relayPlaying<GCPvEItemInit>(m_itemId, m_uid, m_position);
}

void PveItem::spawn(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer) const
{
	roomSessionPlayer->send<GCPvEItemInit>(m_itemId, m_uid, m_position);
}

void PveItem::setUid(const uint32_t uid)
{
	m_uid = uid;
}

void PveItem::setIsPickedUp(const bool value)
{
	m_isPickedUp = value;
}

uint32_t PveItem::getUid() const
{
	return m_uid;
}

uint32_t PveItem::getItemId() const
{
	return m_itemId;
}

Position PveItem::getPosition() const
{
	return m_position;
}

bool PveItem::isPickedUp() const
{
	return m_isPickedUp;
}
