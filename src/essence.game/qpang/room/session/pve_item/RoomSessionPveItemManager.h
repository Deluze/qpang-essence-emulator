#pragma once

#include <memory>
#include <map>

#include "Position.h"

class RoomSession;

class RoomSessionPveItemManager
{
public:
	struct Item
	{
		uint32_t id;
		Position position;
	};

	/**
	 * \brief Initializes the pve item manager with the room session.
	 * \param roomSession The given room session.
	 */
	void initialize(const std::shared_ptr<RoomSession>& roomSession);

	/**
	 * \brief Spawns in an item.
	 * \param item The item you want to spawn in.
	 * \returns The uid of the spawned in item.
	 */
	uint32_t spawnItem(Item item);

	/**
	 * \brief Handles the CGPveGetItem event (item pickup).
	 * \param playerId The id of the player that triggers the event.
	 * \param itemUid The uid of the item that is being picked up.
	 */
	void onItemPickup(uint32_t playerId, uint32_t itemUid);

	/**
	 * \brief Looks up the item by uid.
	 * \param itemUid The uid of the item you are looking for.
	 * \return The item if it was found otherwise null.
	 */
	Item* findItemByUid(uint32_t itemUid);
private:
	std::weak_ptr<RoomSession> m_roomSession;
	std::map<uint32_t, Item> m_items;
};