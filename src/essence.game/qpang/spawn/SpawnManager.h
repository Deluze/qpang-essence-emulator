#pragma once

#include <cstdint>
#include <vector>
#include <mutex>
#include <unordered_map>

#include "qpang/spawn/Spawn.h"

class SpawnManager
{
public:
	void initialize();
	Spawn getRandomPlazaSpawnLocation();
	Spawn getRandomSpawn(uint8_t map, uint8_t team);
	Spawn getRandomTeleportSpawn(uint8_t map);
	std::vector<Spawn> getItemSpawns(uint8_t map);

	Spawn getEssenceSpawn(uint8_t map);
private:
	std::unordered_map<uint8_t, std::unordered_map<uint8_t, std::vector<Spawn>>> m_spawns; // <map, <team, vector<spawn>>
	std::unordered_map<uint8_t, std::vector<Spawn>> m_itemSpawns;

	std::vector<Spawn> m_plazaSpawns {
		Spawn { 4.039125, 0.053376, 8.051445 },
		Spawn { -30.545277, 1.479657, 1.028372 },
		Spawn { -21.903814, 1.548746, -27.334209 },
		Spawn { 49.823097, 0.051706, -10.295254 },
	};

	std::mutex m_mx;
};