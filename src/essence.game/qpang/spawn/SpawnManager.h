#pragma once

#include <cstdint>
#include <vector>
#include <mutex>
#include <unordered_map>

#include "RoomSession.h"
#include "RoomSessionPlayer.h"
#include "qpang/spawn/Spawn.h"

class SpawnManager
{
public:
	void initialize();

	Spawn getLeastPopulatedSpawn(const uint8_t map, const uint8_t team,
	                             const std::vector<RoomSessionPlayer::Ptr>& players,
	                             const RoomSession::Ptr& roomSession);
	Spawn getRandomSpawn(uint8_t map, uint8_t team);
	Spawn getRandomTeleportSpawn(uint8_t map);

	std::vector<Spawn> getItemSpawns(uint8_t map);

	Spawn getEssenceSpawn(uint8_t map);
private:
	std::unordered_map<uint8_t, std::unordered_map<uint8_t, std::vector<Spawn>>> m_spawns; // <map, <team, vector<spawn>>
	std::unordered_map<uint8_t, std::vector<Spawn>> m_itemSpawns;

	std::mutex m_mx;
};