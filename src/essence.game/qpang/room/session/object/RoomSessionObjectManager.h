#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "Position.h"

class RoomSession;
class RoomSessionPlayer;

class RoomSessionObjectManager
{
public:
	// TODO: Move to separate/own file.
	struct Object
	{
		uint32_t type;
		Position position;
	};

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
	uint32_t spawnObject(Object object);

	/**
	 * \brief De-spawns an object from the game and removes it from the objects map.
	 * \param uid The uid of the object to de-spawn.
	 */
	void despawnObject(uint32_t uid);

	/**
	 * \brief Looks up the object by uid.
	 * \param uid The uid of the object you want to find.
	 * \return The found object or null.
	 */
	Object* findObjectByUid(uint32_t uid);

	void onPlayerSync(std::shared_ptr<RoomSessionPlayer> session);

	void tick();
private:
	std::weak_ptr<RoomSession> m_roomSession;
	std::unordered_map<uint32_t, Object> m_objects{};
};
