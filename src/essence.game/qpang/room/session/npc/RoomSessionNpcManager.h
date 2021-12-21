#pragma once

#include <memory>
#include <map>

#include "CGPvEHitNpcData.h"
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

	void tick(const std::shared_ptr<RoomSession>& roomSession) const;

	/**
	 * \brief Temporary function for spawning initial npcs.
	 */
	void spawnInitialNpcs();

	/**
	 * \brief Spawns in an npc.
	 * \param npc The npc you want to spawn in.
	 * \returns The uid of the spawned in npc.
	 */
	uint32_t spawnNpc(const std::shared_ptr<PveNpc>& npc);

	/**
	 * \brief Kills the npc.
	 * \param uid The uid of the npc to de-spawn.
	 */
	void killNpcByUid(uint32_t uid);

	/**
	 * \brief Looks up the npc by uid.
	 * \param uid The uid of the npc you want to find.
	 * \return The found npc or null.
	 */
	std::shared_ptr<PveNpc> findNpcByUid(uint32_t uid);

	/**
	 * \brief Handles npc synchronization upon joining a match.
	 * \param session The player in session.
	 */
	void onPlayerSync(const std::shared_ptr<RoomSessionPlayer>& session) const;

	/**
	 * \brief Handles the onCGPvEHitNpc which occurs when a player hits/damages an npc.
	 * \param data The corresponding data struct for this net event.
	 */
	void onCGPvEHitNpc(const CGPvEHitNpcData& data);
private:
	std::weak_ptr<RoomSession> m_roomSession;
	std::unordered_map<uint32_t, std::shared_ptr<PveNpc>> m_npcs{};
};
