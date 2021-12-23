#pragma once

#include <memory>
#include <unordered_map>

#include <mutex>

#include <qpang/room/session/object/objects/PveObject.h>

class RoomSession;
class RoomSessionPlayer;

class RoomSessionObjectManager
{
public:
	/**
	 * \brief Initializes the object manager with the room session.
	 * \param roomSession The given room session.
	 */
	void initialize(const std::shared_ptr<RoomSession>& roomSession);

	/**
	 * \brief Spawns in an object.
	 * \param object The object you want to spawn in.
	 * \returns The uid of the spawned in object.
	 */
	uint32_t spawnObject(std::shared_ptr<PveObject> object);

	/**
	 * \brief De-spawns an object from the game and removes it from the objects map.
	 * \param uid The uid of the object to de-spawn.
	 */
	void despawnObject(uint32_t uid);

	/**
	 * \brief Destroys the object and removes it from the map.
	 * \param uid The uid of the object to destroy.
	 */
	void destroyObject(uint32_t uid);

	/**
	 * \brief Relays a gc_pve_door event to everyone to open the door/gate.
	 * \param uid The uid of the object/gate you want to open.
	 */
	void openGate(uint32_t uid) const;

	/**
	 * \brief Relays a gc_pve_door event to everyone to close the door/gate.
	 * \param uid The uid of the object/gate you want to close.
	 */
	void closeGate(uint32_t uid) const;

	/**
	 * \brief Looks up the object by uid.
	 * \param uid The uid of the object you want to find.
	 * \return The found object or null.
	 */
	std::shared_ptr<PveObject> findObjectByUid(uint32_t uid);

	void onPlayerSync(const std::shared_ptr<RoomSessionPlayer>& session);

	void tick() const;
private:
	std::mutex mutex;
	std::weak_ptr<RoomSession> m_roomSession;
	std::unordered_map<uint32_t, std::shared_ptr<PveObject>> m_objects{};
};
