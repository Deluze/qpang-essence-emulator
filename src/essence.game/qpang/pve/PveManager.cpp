#include "PveManager.h"

#include "core/Emulator.h"
#include "core/database/Database.h"

void PveManager::initialize()
{
	std::cout << "Initializing the PveManager." << std::endl;

	initializeNpcs();
	initializeObjects();
}

std::vector<PveNpc> PveManager::getNpcsByMapId(const uint8_t mapId)
{
	const auto& it = m_npcs.find(mapId);

	if (it == m_npcs.end())
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
	std::cout << "Initializing the PveManager npcs." << std::endl;

	// Step 1: Get all npcs from pve_npc_spawns.
	const auto result = DATABASE->prepare(
		"SELECT "
		"pve_npcs.id AS Id "
		",pve_npcs.type AS Type "
		",pve_npcs.base_health AS BaseHealth "
		",pve_npcs.speed AS Speed "
		",pve_npcs.weapon_item_id AS WeaponItemId "
		",pve_npcs.weapon_body_part_id AS WeaponBodyPartId "
		",pve_npcs.ai_time AS AITime "
		",pve_npcs.attack_width AS AttackWidth "
		",pve_npcs.attack_height AS AttackHeight "
		",pve_npcs.can_drop_loot AS CanDropLoot "
		",pve_npc_spawns.should_respawn AS ShouldRespawn "
		",pve_npc_spawns.respawn_time AS RespawnTime "
		",pve_npc_spawns.initial_rotation AS InitialRotation "
		",pve_npc_spawns.spawn_position_x AS SpawnPositionX "
		",pve_npc_spawns.spawn_position_y AS SpawnPositionY "
		",pve_npc_spawns.spawn_position_z AS SpawnPositionZ "
		",pve_npc_movement_types.type AS MovementType "
		",pve_npc_target_types.type AS TargetType "
		",pve_npc_grade_types.type AS GradeType "
		",maps.map_id AS MapId "
		"FROM pve_npc_spawns "
		"INNER JOIN pve_npcs ON pve_npcs.id = pve_npc_spawns.npc_id "
		"INNER JOIN pve_npc_movement_types ON pve_npc_movement_types.id = pve_npcs.movement_type_id "
		"INNER JOIN pve_npc_target_types ON pve_npc_target_types.id = pve_npcs.target_type_id "
		"INNER JOIN pve_npc_grade_types ON pve_npc_grade_types.id = pve_npcs.grade_type_id "
		"INNER JOIN maps on maps.id = pve_npc_spawns.map_id;"
	)->fetch();

	m_npcs.clear();

	while(result->hasNext())
	{
		const auto npcPrimaryKey = result->getInt("Id");

		// Step 2: Get the loot for every npc.
		const auto lootDrops = getLootDropsByNpcPrimaryKey(npcPrimaryKey);

		// Step 3: Get the body parts for every npc. 
		const auto bodyParts = getBodyPartsByNpcPrimaryKey(npcPrimaryKey);

		// Step 4: Construct the npc.
		auto npc = PveNpc(
			result->getTiny("Type"),
			result->getShort("BaseHealth"),
			result->getFloat("Speed"),
			result->getInt("WeaponItemId"),
			result->getTiny("WeaponBodyPartId"),
			result->getInt("AITime"),
			result->getFloat("AttackWidth"),
			result->getFloat("AttackHeight"),
			result->getInt("ShouldRespawn"),
			result->getInt("RespawnTime"),
			result->getFlag("CanDropLoot"),
			result->getShort("InitialRotation"),
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
			);

		// Step 5: Save the npcs to the map.
		m_npcs[result->getInt("MapId")].push_back(npc);

		result->next();
	}
}

std::vector<NpcLootDrop> PveManager::getLootDropsByNpcPrimaryKey(const uint32_t npcPrimaryKey)
{
	const auto statement = DATABASE->prepare("SELECT * FROM pve_npc_loot_drops WHERE npc_id = ?");

	statement->bindInteger(npcPrimaryKey);

	const auto result = statement->fetch();

	std::vector<NpcLootDrop> lootDrops{};

	while(result->hasNext())
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
	const auto statement = DATABASE->prepare("SELECT * FROM pve_npc_body_parts WHERE npc_id = ?");

	statement->bindInteger(npcPrimaryKey);

	const auto result = statement->fetch();

	std::vector<NpcBodyPart> bodyParts{};

	while (result->hasNext())
	{
		auto bodyPart = NpcBodyPart
		{
			result->getInt("body_part_id"),
			result->getShort("health"),
			result->getInt("weapon_item_id"),
			result->getInt("item_box_id"),
			result->getFlag("is_dual_gun")
		};

		bodyParts.push_back(bodyPart);

		result->next();
	}

	return bodyParts;
}

void PveManager::initializeObjects()
{

}
