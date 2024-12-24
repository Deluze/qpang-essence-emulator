#include "SpawnManager.h"

#include <iostream>
#include <ctime>

#include "AABBHelper.h"
#include "RoomSession.h"
#include "core/Emulator.h"
#include "core/database/Database.h"

void SpawnManager::initialize()
{
	std::lock_guard<std::mutex> lg(m_mx);

	srand(time(NULL));

	auto res = DATABASE->prepare(
		"SELECT * FROM map_spawns INNER JOIN positions ON positions.id = map_spawns.position_id INNER JOIN maps ON maps.id = map_spawns.map_id INNER JOIN game_modes ON game_modes.id = map_spawns.game_mode_id WHERE map_spawns.is_active = 1"
	)->fetch();

	std::cout << "Loading player spawns.\n";

	m_spawns.clear();
	m_itemSpawns.clear();

	while (res->hasNext())
	{
		Spawn sp{
			res->getFloat("x"),
			res->getFloat("y"),
			res->getFloat("z"),
		};

		auto team = res->getTiny("team");
		auto map = res->getTiny("map_id");

		m_spawns[map][team].push_back(sp);

		res->next();
	}

	std::cout << "Loading game item spawns.\n";

	res = DATABASE->prepare(
		"SELECT game_item_spawns.id, maps.map_id, positions.x, positions.y, positions.z FROM game_item_spawns INNER JOIN positions ON positions.id = game_item_spawns.position_id INNER JOIN maps ON maps.id = game_item_spawns.map_id where is_active = 1"
	)->fetch();

	while (res->hasNext())
	{
		Spawn sp{
			res->getFloat("x"),
			res->getFloat("y"),
			res->getFloat("z"),
		};

		m_itemSpawns[res->getTiny("map_id")].push_back(sp);

		res->next();
	}
}

Spawn SpawnManager::getLeastPopulatedSpawn(const uint8_t map, const uint8_t team,
	const std::vector<RoomSessionPlayer::Ptr>& players, const RoomSession::Ptr& roomSession)
{
	std::lock_guard lg(m_mx);

	// The team must be solo (0), blue (1) or yellow (2) for it to be valid.
	if (team != 0 && team != 1 && team != 2)
	{
		return {};
	}

	const auto& spawns = m_spawns[map][team];

	// If there are no spawns we return an empty struct.
	if (spawns.empty())
	{
		return {};
	}

	std::unordered_map<uint32_t, uint32_t> spawnPopulations{};

	uint32_t lowestSpawnPopulationCount = std::numeric_limits<uint32_t>::max();

	for (uint32_t i = 0; i < spawns.size(); i++)
	{
		const auto& spawn = spawns[i];

		spawnPopulations[i] = 0;

		for (const auto& player : players)
		{
			const auto playerPosition = player->getPosition();

			// ReSharper disable once CppTooWideScope
			const auto isWithinSpawnRadius = AABBHelper::IsPositionWithinSpawnRadius(playerPosition, spawn, 15);

			if (isWithinSpawnRadius)
			{
				spawnPopulations[i]++;
			}
		}

		if (spawnPopulations[i] < lowestSpawnPopulationCount)
		{
			lowestSpawnPopulationCount = spawnPopulations[i];
		}
	}

	uint32_t spawnIndex = 0;
	std::vector<Spawn> lowestSpawnPopulations{};

	for (const auto& spawnPopulation : spawnPopulations)
	{
		if (spawnPopulation.second == lowestSpawnPopulationCount)
		{
			lowestSpawnPopulations.push_back(spawns[spawnIndex]);
		}

		spawnIndex++;
	}

	const auto lastRespawnLocation = roomSession->getLastRespawnLocation();

	auto findRespawnLocationAttempt = 1;
	auto newRespawnLocation = lastRespawnLocation;

	while ((findRespawnLocationAttempt <= 5) && lastRespawnLocation.x == newRespawnLocation.x && lastRespawnLocation.y == newRespawnLocation.y && lastRespawnLocation.z == newRespawnLocation.z)
	{
		newRespawnLocation = lowestSpawnPopulations[rand() % lowestSpawnPopulations.size()];

		findRespawnLocationAttempt++;
	}

	return newRespawnLocation;
}

Spawn SpawnManager::getRandomSpawn(uint8_t map, uint8_t team)
{
	std::lock_guard<std::mutex> lg(m_mx);

	if (team != 0 && team != 1 && team != 2)
		return {};

	auto& spawns = m_spawns[map][team];

	if (spawns.empty())
		return {};

	auto index = rand() % spawns.size();

	return spawns[index];
}

Spawn SpawnManager::getRandomTeleportSpawn(uint8_t map)
{
	std::lock_guard<std::mutex> lg(m_mx);

	auto spawns = m_spawns[map][98];

	if (spawns.empty())
		return { 0xFF, 0xFF, 0xFF };

	auto index = rand() % spawns.size();

	return spawns[index];
}

std::vector<Spawn> SpawnManager::getItemSpawns(uint8_t map)
{
	std::lock_guard<std::mutex> lg(m_mx);

	return m_itemSpawns[map];
}

Spawn SpawnManager::getEssenceSpawn(uint8_t map)
{
	std::lock_guard<std::mutex> lg(m_mx);

	auto spawns = m_spawns[map][99];

	if (spawns.empty())
		return { 0, 0 , 0 };

	return spawns[0];
}
