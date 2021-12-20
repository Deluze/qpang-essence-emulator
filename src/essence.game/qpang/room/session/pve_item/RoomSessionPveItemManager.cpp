#include "RoomSessionPveItemManager.h"

#include "RoomSessionPlayer.h"

#include "gc_game_item.hpp"
#include "gc_pve_item_init.hpp"

void RoomSessionPveItemManager::initialize(const std::shared_ptr<RoomSession>& roomSession)
{
	m_roomSession = roomSession;
}

uint32_t RoomSessionPveItemManager::spawnItem(PveItem item)
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return 0;
	}

	item.setUid((m_items.size() + 1));

	roomSession->relayPlaying<GCPvEItemInit>(item.getType(), item.getUid(), item.getPosition());

	m_items[item.getUid()] = item;

	return item.getUid();
}

void RoomSessionPveItemManager::onItemPickup(const uint32_t playerId, const uint32_t itemUid)
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}
	const auto roomSessionPlayer = roomSession->find(playerId);

	if ( roomSessionPlayer == nullptr)
	{
		return;
	}

	const auto item = findItemByUid(itemUid);

	if (item == nullptr)
	{
		return;
	}

	// TODO: Increase player coin count based on coin type (if it even is a coin).

	roomSession->relayPlaying<GCGameItem>(GCGameItem::CMD::PICKUP_GAME_ITEM, playerId, static_cast<U32>(item->getType()), itemUid, 0);

	m_items.erase(itemUid);
}

PveItem* RoomSessionPveItemManager::findItemByUid(const uint32_t itemUid)
{
	const auto it = m_items.find(itemUid);

	if (it == m_items.end())
	{
		return nullptr;
	}

	return &it->second;
}

void RoomSessionPveItemManager::onPlayerSync(const std::shared_ptr<RoomSessionPlayer>& session) const
{
	for (const auto& item : m_items)
	{
		session->send<GCPvEItemInit>(item.second.getType(), item.first, item.second.getPosition());
	}
}

void RoomSessionPveItemManager::tick(const std::shared_ptr<RoomSession>& roomSession) const
{

}