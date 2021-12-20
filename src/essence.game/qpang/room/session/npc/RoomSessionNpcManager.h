#pragma once

#include <memory>
#include <map>

#include "PveNpc.h"

class RoomSession;
class RoomSessionPlayer;

class RoomSessionNpcManager
{
public:
	/**
	 * \brief Initializes the npc manager with the room session.
	 * \param roomSession The given room session.
	 */
	void initialize(const std::shared_ptr<RoomSession>& roomSession);

	/**
	 * \brief Temporary function for spawning initial npcs.
	 */
	void spawnInitialNpcs();

	/**
	 * \brief Spawns in an npc.
	 * \param npc The npc you want to spawn in.
	 * \returns The uid of the spawned in npc.
	 */
	uint32_t spawnNpc(PveNpc npc);

	/**
	 * \brief De-spawns an from the game and removes it from the npcs vector.
	 * \param uid The uid of the npc to de-spawn.
	 */
	void killNpcByUid(uint32_t uid);

	/**
	 * \brief Looks up the npc by uid.
	 * \param uid The uid of the npc you want to find.
	 * \return The found npc or null.
	 */
	PveNpc* findNpcByUid(uint32_t uid);

	void onPlayerSync(const std::shared_ptr<RoomSessionPlayer>& session) const;

	void tick(const std::shared_ptr<RoomSession>& roomSession) const;
private:
	std::weak_ptr<RoomSession> m_roomSession;
	std::map<uint32_t, PveNpc> m_npcs{};
};
