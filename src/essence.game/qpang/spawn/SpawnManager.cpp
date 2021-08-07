#include "SpawnManager.h"

#include <iostream>
#include <ctime>

#include "core/Emulator.h"
#include "core/database/Database.h"

void SpawnManager::initialize()
{
	std::lock_guard<std::mutex> lg(m_mx);

	srand(time(NULL));

	auto res = DATABASE->prepare(
		"SELECT * FROM map_spawns INNER JOIN positions ON positions.id = map_spawns.position_id INNER JOIN maps ON maps.id = map_spawns.map_id INNER JOIN game_modes ON game_modes.id = map_spawns.game_mode_id"
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
		"SELECT game_item_spawns.id, maps.map_id, positions.x, positions.y, positions.z FROM game_item_spawns INNER JOIN positions ON positions.id = game_item_spawns.position_id INNER JOIN maps ON maps.id = game_item_spawns.map_id"
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

Spawn SpawnManager::getRandomPlazaSpawnLocation()
{
	std::lock_guard<std::mutex> lg(m_mx);

	if (m_plazaSpawns.empty()) {
		// TODO: Fallback location.
		return {};
	}

	auto index = rand() % m_plazaSpawns.size();

	return m_plazaSpawns[index];
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
