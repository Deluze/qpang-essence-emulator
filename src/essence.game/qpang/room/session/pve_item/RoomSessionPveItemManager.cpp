#include "RoomSessionPveItemManager.h"

#include "AABBHelper.h"
#include "gc_game_item.hpp"
#include "gc_pve_item_init.hpp"

void RoomSessionPveItemManager::initialize(const std::shared_ptr<RoomSession>& roomSession)
{
	m_roomSession = roomSession;
}

uint32_t RoomSessionPveItemManager::spawnItem(const Item item)
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return 0;
	}

	const auto itemUid = (m_items.size() + 1);

	roomSession->relayPlaying<GCPvEItemInit>(item.id, itemUid, item.position);

	m_items[itemUid] = item;

	return itemUid;
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

	/*const auto distance = AABBHelper::getDistanceBetweenPositions(roomSessionPlayer->getPosition(), item->position);

	// TODO: Determine the best distance value.

	if (distance > 5)
	{
		return;
	}*/

	// TODO: Increase player coin count based on coin type (if it even is a coin).

	roomSession->relayPlaying<GCGameItem>(GCGameItem::CMD::PICKUP_GAME_ITEM, playerId, item->id, itemUid, 0);
}

RoomSessionPveItemManager::Item* RoomSessionPveItemManager::findItemByUid(const uint32_t itemUid)
{
	const auto it = m_items.find(itemUid);

	if (it == m_items.end())
	{
		return nullptr;
	}

	return &it->second;
}
