#pragma once

#include <memory>
#include <map>

#include "Position.h"

class RoomSession;

class RoomSessionNpcManager
{
public:
	// TODO: Move to separate/own file.
	struct Npc
	{
		uint32_t type;
		Position position;
	};

	/**
	 * \brief Initializes the npc manager with the room session.
	 * \param roomSession The given room session.
	 */
	void initialize(const std::shared_ptr<RoomSession>& roomSession);

	/**
	 * \brief Spawns in an npc.
	 * \param npc The npc you want to spawn in.
	 * \returns The uid of the spawned in npc.
	 */
	uint32_t spawnNpc(Npc npc);

	/**
	 * \brief De-spawns an from the game and removes it from the npcs vector.
	 * \param uid The uid of the npc to de-spawn.
	 */
	void despawnNpc(uint32_t uid);

	/**
	 * \brief Looks up the npc by uid.
	 * \param uid The uid of the npc you want to find.
	 * \return The found npc or null.
	 */
	Npc* findNpcByUid(uint32_t uid);
private:
	std::weak_ptr<RoomSession> m_roomSession;
	std::map<uint32_t, Npc> m_npcs{};
};
