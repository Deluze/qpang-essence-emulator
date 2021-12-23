#pragma once

#include <memory>
#include <map>

#include "PveItem.h"
#include "RoomSessionPlayer.h"

class RoomSession;
class RoomSessionPlayer;

class RoomSessionPveItemManager
{
public:
	/**
	 * \brief Initializes the pve item manager with the room session.
	 * \param roomSession The given room session.
	 */
	void initialize(const std::shared_ptr<RoomSession>& roomSession);

	void tick() const;

	/**
	 * \brief Spawns in an item.
	 * \param item The item you want to spawn in.
	 * \returns The uid of the spawned in item.
	 */
	uint32_t spawnItem(const std::shared_ptr<PveItem>& item);

	/**
	 * \brief Handles the CGPveGetItem event (item pickup).
	 * \param playerId The id of the player that triggers the event.
	 * \param uid The uid of the item that is being picked up.
	 */
	void onItemPickup(uint32_t playerId, uint32_t uid);

	/**
	 * \brief Looks up the item by uid.
	 * \param itemUid The uid of the item you are looking for.
	 * \return The item if it was found otherwise null.
	 */
	std::shared_ptr<PveItem> findItemByUid(uint32_t itemUid);

	void onPlayerSync(const std::shared_ptr<RoomSessionPlayer>& session) const;
private:
	/**
	 * \brief Handles the item pick up event for ammo.
	 */
	static void handleAmmoPickup(const RoomSessionPlayer::Ptr& roomSessionPlayer);

	/**
	 * \brief Handles the item pick up event for med kits.
	 */
	static void handleMedkitPickup(const RoomSessionPlayer::Ptr& roomSessionPlayer);

	/**
	 * \brief Handles the item pick up event for coins.
	 */
	static void handleCoinPickup(const RoomSessionPlayer::Ptr& roomSessionPlayer, eItemType itemType);

	std::weak_ptr<RoomSession> m_roomSession;
	std::map<uint32_t, std::shared_ptr<PveItem>> m_items;
};