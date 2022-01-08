#include "PveManager.h"

#include "MoveableObject.h"
#include "core/Emulator.h"
#include "core/database/Database.h"

void PveManager::initialize()
{
	std::cout << "Initializing the PveManager." << std::endl;

	initializeNpcs();
	initializeObjects();
	initializeItems();
	initializeAreas();
	initializeNpcWaves();
}

std::vector<PveNpcData> PveManager::getNpcDataByMapId(const uint8_t mapId)
{
	const auto& it = m_npcData.find(mapId);

	if (it == m_npcData.end())
	{
		return {};
	}

	if (it->second.empty())
	{
		return {};
	}

	return it->second;
}

std::unordered_map<uint8_t, std::vector<PveNpcWaveData>> PveManager::getNpcWaveData(const uint8_t mapId)
{
	const auto& mapIt = m_npcWaveData.find(mapId);

	if (mapIt == m_npcWaveData.end())
	{
		return {};
	}

	return mapIt->second;
}

std::vector<PveObjectData> PveManager::getObjectDataByMapId(const uint8_t mapId)
{
	const auto& it = m_objectData.find(mapId);

	if (it == m_objectData.end())
	{
		return {};
	}

	if (it->second.empty())
	{
		return {};
	}

	return it->second;
}

std::vector<PveItemData> PveManager::getItemDataByMapId(const uint8_t mapId)
{
	const auto& it = m_itemData.find(mapId);

	if (it == m_itemData.end())
	{
		return {};
	}

	if (it->second.empty())
	{
		return {};
	}

	return it->second;
}

std::vector<PveAreaData> PveManager::getAreaDataByMapId(const uint8_t mapId)
{
	const auto& it = m_areaData.find(mapId);

	if (it == m_areaData.end())
	{
		return {};
	}

	if (it->second.empty())
	{
		return {};
	}

	return it->second;
}

void PveManager::initializeNpcs()
{
	std::cout << "Initializing pve npc data." << std::endl;

	// Step 1: Get all npcs from pve_npc_spawns.
	const auto result = DATABASE->prepare(
		"SELECT "
		"pve_npcs.id AS Id "
		",pve_npcs.type AS Type "
		",pve_npcs.base_health AS BaseHealth "
		",pve_npcs.speed AS Speed "
		",pve_npcs.weapon_item_id AS WeaponItemId "
		",pve_npcs.weapon_body_part_id AS WeaponBodyPartId "
		",pve_npcs.weapon_type AS WeaponType "
		",pve_npcs.ai_time AS AITime "
		",pve_npcs.attack_range AS AttackRange "
		",pve_npcs.attack_width AS AttackWidth "
		",pve_npcs.attack_height AS AttackHeight "
		",pve_npcs.can_drop_loot AS CanDropLoot "
		",pve_npc_spawns.should_respawn AS ShouldRespawn "
		",pve_npc_spawns.respawn_time AS RespawnTime "
		",pve_npc_spawns.initial_rotation AS InitialRotation "
		",pve_npc_spawns.spawn_position_x AS SpawnPositionX "
		",pve_npc_spawns.spawn_position_y AS SpawnPositionY "
		",pve_npc_spawns.spawn_position_z AS SpawnPositionZ "
		",pve_npc_spawns.static_shooting_position_x AS StaticShootingPositionX "
		",pve_npc_spawns.static_shooting_position_y AS StaticShootingPositionY "
		",pve_npc_spawns.static_shooting_position_z AS StaticShootingPositionZ "
		",pve_npc_movement_types.type AS MovementType "
		",pve_npc_target_types.type AS TargetType "
		",pve_npc_grade_types.type AS GradeType "
		",pve_map_areas.area_uid AS AreaUid "
		",pve_map_areas.floor_number AS FloorNumber "
		",maps.map_id AS MapId "
		"FROM pve_npc_spawns "
		"INNER JOIN pve_npcs ON pve_npcs.id = pve_npc_spawns.npc_id "
		// TODO: Maybe check pve_map_areas id and map_id.
		"INNER JOIN pve_map_areas ON pve_map_areas.id = pve_npc_spawns.area_id "
		"INNER JOIN pve_npc_movement_types ON pve_npc_movement_types.id = pve_npcs.movement_type_id "
		"INNER JOIN pve_npc_target_types ON pve_npc_target_types.id = pve_npcs.target_type_id "
		"INNER JOIN pve_npc_grade_types ON pve_npc_grade_types.id = pve_npcs.grade_type_id "
		"INNER JOIN maps on maps.id = pve_npc_spawns.map_id;"
	)->fetch();

	m_npcData.clear();

	while (result->hasNext())
	{
		const auto npcPrimaryKey = result->getInt("Id");

		// Step 2: Get the loot for every npc.
		const auto lootDrops = getLootDropsByNpcPrimaryKey(npcPrimaryKey);

		// Step 3: Get the body parts for every npc. 
		const auto bodyParts = getBodyPartsByNpcPrimaryKey(npcPrimaryKey);

		// Step 4: Construct the npc.
		auto npcData = PveNpcData{
			result->getInt("AreaUid"),
			result->getInt("FloorNumber"),
			result->getTiny("Type"),
			result->getShort("BaseHealth"),
			result->getFloat("Speed"),
			result->getInt("WeaponItemId"),
			result->getTiny("WeaponBodyPartId"),
			static_cast<eWeaponType>(result->getInt("WeaponType")),
			result->getInt("AITime"),
			result->getFloat("AttackRange"),
			result->getFloat("AttackWidth"),
			result->getFloat("AttackHeight"),
			result->getFlag("ShouldRespawn"),
			result->getInt("RespawnTime"),
			result->getFlag("CanDropLoot"),
			result->getShort("InitialRotation"),
			Position{
				result->getFloat("SpawnPositionX"),
				result->getFloat("SpawnPositionY"),
				result->getFloat("SpawnPositionZ"),
			},
			Position{
				result->getFloat("StaticShootingPositionX"),
				result->getFloat("StaticShootingPositionY"),
				result->getFloat("StaticShootingPositionZ"),
			},
			static_cast<eNpcGradeType>(result->getInt("GradeType")),
			static_cast<eNpcMovementType>(result->getInt("MovementType")),
			static_cast<eNpcTargetType>(result->getInt("TargetType")),
			lootDrops,
			bodyParts
		};

		const auto mapId = result->getInt("MapId");

		// Step 5: Save the npcs to the map.
		m_npcData[mapId].push_back(npcData);

		result->next();
	}
}

std::vector<NpcLootDrop> PveManager::getLootDropsByNpcPrimaryKey(const uint32_t npcPrimaryKey)
{
	const auto statement = DATABASE->prepare("SELECT * FROM pve_npc_loot_drops WHERE npc_id = ?");

	statement->bindInteger(npcPrimaryKey);

	const auto result = statement->fetch();

	std::vector<NpcLootDrop> lootDrops{};

	while (result->hasNext())
	{
		auto lootDrop = NpcLootDrop
		{
			result->getInt("item_id"),
			result->getInt("probability")
		};

		lootDrops.push_back(lootDrop);

		result->next();
	}

	return lootDrops;
}

std::vector<NpcBodyPart> PveManager::getBodyPartsByNpcPrimaryKey(const uint32_t npcPrimaryKey)
{
	const auto statement = DATABASE->prepare(
		"SELECT "
		"pve_npc_body_parts.body_part_id AS BodyPartId "
		",pve_npc_body_parts.health AS Health "
		",pve_npc_body_parts.weapon_item_id AS WeaponItemId "
		",pve_npc_body_parts.item_box_id AS ItemBoxId "
		",pve_npc_body_parts.is_dual_gun AS IsDualGun "
		",pve_body_part_attribute_types.type AS AttributeType "
		",pve_body_part_damage_types.type AS DamageType "
		",pve_body_part_part_types.type AS PartType "
		"FROM pve_npc_body_parts "
		"INNER JOIN pve_body_parts ON pve_body_parts.id = pve_npc_body_parts.body_part_id "
		"INNER JOIN pve_body_part_attribute_types ON pve_body_part_attribute_types.id = pve_body_parts.attribute_type_id "
		"INNER JOIN pve_body_part_damage_types ON pve_body_part_damage_types.id = pve_body_parts.damage_type_id "
		"INNER JOIN pve_body_part_part_types ON pve_body_part_part_types.id = pve_body_parts.part_type_id "
		"WHERE pve_npc_body_parts.npc_id = ? "
		"ORDER BY pve_npc_body_parts.body_part_id;"
	);

	statement->bindInteger(npcPrimaryKey);

	const auto result = statement->fetch();

	std::vector<NpcBodyPart> bodyParts{};

	while (result->hasNext())
	{
		auto bodyPart = NpcBodyPart
		{
			result->getInt("BodyPartId"),
			result->getShort("Health"),
			result->getInt("WeaponItemId"),
			result->getInt("ItemBoxId"),
			result->getFlag("IsDualGun"),
			static_cast<eNpcBodyPartAttributeType>(result->getTiny("AttributeType")),
			static_cast<eNpcBodyPartDamageType>(result->getTiny("DamageType")),
			static_cast<eNpcBodyPartPartType>(result->getTiny("PartType")),
		};

		bodyParts.push_back(bodyPart);

		result->next();
	}

	return bodyParts;
}

void PveManager::initializeObjects()
{
	std::cout << "Initializing pve object data." << std::endl;

	const auto result = DATABASE->prepare(
		"SELECT "
		"pve_object_spawns.id AS ObjectUid "
		",pve_object_spawns.type AS ObjectType "
		",pve_object_spawns.spawn_position_x AS SpawnPositionX "
		",pve_object_spawns.spawn_position_y AS SpawnPositionY "
		",pve_object_spawns.spawn_position_z AS SpawnPositionZ "
		",pve_object_spawns.end_position_x AS EndPositionX "
		",pve_object_spawns.end_position_y AS EndPositionY "
		",pve_object_spawns.end_position_z AS EndPositionZ "
		",pve_object_spawns.initial_health AS InitialHealth "
		",pve_object_spawns.is_moveable AS IsMoveable "
		",pve_object_spawns.move_duration AS MoveDuration "
		",pve_object_spawns.move_wait AS MoveWait "
		",pve_object_spawns.linked_object AS LinkedObject "
		",maps.map_id AS MapId "
		"FROM pve_object_spawns "
		"INNER JOIN maps ON maps.id = pve_object_spawns.map_id;"
	)->fetch();

	m_objectData.clear();

	while (result->hasNext())
	{
		auto objectData = PveObjectData
		{
			result->getInt("ObjectUid"),
			static_cast<eObjectType>(result->getInt("ObjectType")),
			Position{
				result->getFloat("SpawnPositionX"),
				result->getFloat("SpawnPositionY"),
				result->getFloat("SpawnPositionZ"),
			},
			Position{
				result->getFloat("EndPositionX"),
				result->getFloat("EndPositionY"),
				result->getFloat("EndPositionZ"),
			},
			result->getShort("InitialHealth"),
			result->getFlag("IsMoveable"),
			result->getInt("MoveDuration"),
			result->getInt("MoveWait"),
			result->getInt("LinkedObject")
		};

		const auto mapId = result->getInt("MapId");

		m_objectData[mapId].push_back(objectData);

		result->next();
	}
}

void PveManager::initializeItems()
{
	std::cout << "Initializing pve item data." << std::endl;

	const auto result = DATABASE->prepare(
		"SELECT "
		"position_x AS PositionX "
		",position_y AS PositionY "
		",position_z AS PositionZ "
		",pve_item_spawn_types.type AS SpawnType "
		",maps.map_id AS MapId "
		"FROM pve_item_spawns "
		"INNER JOIN pve_item_spawn_types ON pve_item_spawn_types.id = pve_item_spawns.item_spawn_type_id "
		"INNER JOIN maps ON maps.id = pve_item_spawns.map_id;"
	)->fetch();

	m_itemData.clear();

	while (result->hasNext())
	{
		auto itemData = PveItemData
		{
			static_cast<eItemSpawnType>(result->getTiny("SpawnType")),
			Position{
				result->getFloat("PositionX"),
				result->getFloat("PositionY"),
				result->getFloat("PositionZ")
			}
		};

		const auto mapId = result->getInt("MapId");

		m_itemData[mapId].push_back(itemData);

		result->next();
	}
}

void PveManager::initializeAreas()
{
	std::cout << "Initializing pve area data." << std::endl;

	const auto result = DATABASE->prepare(
		"SELECT "
		"pve_map_areas.area_uid AS AreaUid "
		",pve_map_areas.min_bound_x as MinBoundX "
		",pve_map_areas.min_bound_z as MinBoundZ "
		",pve_map_areas.max_bound_x as MaxBoundX "
		",pve_map_areas.max_bound_z as MaxBoundZ "
		",pve_map_areas.floor_number as FloorNumber "
		",pve_map_areas.pass_through_is_required as PassThroughIsRequired "
		",maps.map_id AS MapId "
		"FROM pve_map_areas "
		"INNER JOIN maps ON maps.id = pve_map_areas.map_id;"
	)->fetch();

	m_areaData.clear();

	while (result->hasNext())
	{
		auto areaData = PveAreaData
		{
			result->getInt("AreaUid"),
			Position{
				result->getFloat("MinBoundX"),
				0.00f,
				result->getFloat("MinBoundZ")
			},
			Position{
				result->getFloat("MaxBoundX"),
				0.00f,
				result->getFloat("MaxBoundZ"),
			},
			result->getInt("FloorNumber"),
			result->getFlag("PassThroughIsRequired")
		};

		const auto mapId = result->getInt("MapId");

		m_areaData[mapId].push_back(areaData);

		result->next();
	}
}

void PveManager::initializeNpcWaves()
{
	std::cout << "Initializing pve npc wave data." << std::endl;

	// Step 1: Get all npcs from pve_npc_waves.
	const auto result = DATABASE->prepare(
		"SELECT "
		"pve_npcs.id AS Id "
		",pve_npcs.type AS Type "
		",pve_npcs.base_health AS BaseHealth "
		",pve_npcs.speed AS Speed "
		",pve_npcs.weapon_item_id AS WeaponItemId "
		",pve_npcs.weapon_body_part_id AS WeaponBodyPartId "
		",pve_npcs.weapon_type AS WeaponType "
		",pve_npcs.ai_time AS AITime "
		",pve_npcs.attack_range AS AttackRange "
		",pve_npcs.attack_width AS AttackWidth "
		",pve_npcs.attack_height AS AttackHeight "
		",pve_npcs.can_drop_loot AS CanDropLoot "
		",pve_npc_waves.wave_number AS WaveNumber "
		",pve_npc_waves.spawn_position_x AS SpawnPositionX "
		",pve_npc_waves.spawn_position_y AS SpawnPositionY "
		",pve_npc_waves.spawn_position_z AS SpawnPositionZ "
		",pve_npc_movement_types.type AS MovementType "
		",pve_npc_target_types.type AS TargetType "
		",pve_npc_grade_types.type AS GradeType "
		",maps.map_id AS MapId "
		"FROM pve_npc_waves "
		"INNER JOIN pve_npcs ON pve_npcs.id = pve_npc_waves.npc_id "
		"INNER JOIN pve_npc_movement_types ON pve_npc_movement_types.id = pve_npcs.movement_type_id "
		"INNER JOIN pve_npc_target_types ON pve_npc_target_types.id = pve_npcs.target_type_id "
		"INNER JOIN pve_npc_grade_types ON pve_npc_grade_types.id = pve_npcs.grade_type_id "
		"INNER JOIN maps on maps.id = pve_npc_waves.map_id;"
	)->fetch();

	m_npcWaveData.clear();

	while (result->hasNext())
	{
		const auto npcPrimaryKey = result->getInt("Id");

		// Step 2: Get the loot for every npc.
		const auto lootDrops = getLootDropsByNpcPrimaryKey(npcPrimaryKey);

		// Step 3: Get the body parts for every npc. 
		const auto bodyParts = getBodyPartsByNpcPrimaryKey(npcPrimaryKey);

		// Step 4: Construct the npc.
		auto npcWaveData = PveNpcWaveData{
			result->getTiny("Type"),
			result->getShort("BaseHealth"),
			result->getFloat("Speed"),
			result->getInt("WeaponItemId"),
			result->getTiny("WeaponBodyPartId"),
			static_cast<eWeaponType>(result->getInt("WeaponType")),
			result->getInt("AITime"),
			result->getFloat("AttackRange"),
			result->getFloat("AttackWidth"),
			result->getFloat("AttackHeight"),
			result->getFlag("CanDropLoot"),
			Position{
				result->getFloat("SpawnPositionX"),
				result->getFloat("SpawnPositionY"),
				result->getFloat("SpawnPositionZ"),
			},
			static_cast<eNpcGradeType>(result->getInt("GradeType")),
			static_cast<eNpcMovementType>(result->getInt("MovementType")),
			static_cast<eNpcTargetType>(result->getInt("TargetType")),
			lootDrops,
			bodyParts
		};

		const auto mapId = result->getInt("MapId");
		const auto waveNumber = result->getTiny("WaveNumber");

		// Step 5: Save the npcs to the map.
		m_npcWaveData[mapId][waveNumber].push_back(npcWaveData);

		result->next();
	}
}
