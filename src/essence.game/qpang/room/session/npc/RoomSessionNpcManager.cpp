#include "RoomSessionNpcManager.h"

#include "GCPvEHitNpcData.h"
#include "gc_pve_die_npc.hpp"
#include "gc_pve_hit_npc.hpp"
#include "PveNpc.h"
#include "RoomSession.h"
#include "RoomSessionPlayer.h"

void RoomSessionNpcManager::initialize(const std::shared_ptr<RoomSession>& roomSession)
{
	m_roomSession = roomSession;
}

void RoomSessionNpcManager::onStart()
{
	initializeNpcs();
	spawnInitializedNpcs();
}

void RoomSessionNpcManager::tick() const
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	for (const auto& [uid, npc] : m_spawnedNpcs)
	{
		npc->tick(roomSession);
	}
}

void RoomSessionNpcManager::initializeNpcs()
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	const auto getNpcsStatement = DATABASE->prepare(
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
		"FROM pve_npc_spawns "
		"INNER JOIN pve_npcs ON pve_npcs.id = pve_npc_spawns.npc_id "
		"INNER JOIN pve_npc_movement_types ON pve_npc_movement_types.id = pve_npcs.movement_type_id "
		"INNER JOIN pve_npc_target_types ON pve_npc_target_types.id = pve_npcs.target_type_id "
		"INNER JOIN pve_npc_grade_types ON pve_npc_grade_types.id = pve_npcs.grade_type_id "
		"INNER JOIN maps on maps.id = pve_npc_spawns.map_id "
		"WHERE maps.map_id = ?;"
	);

	getNpcsStatement->bindInteger(roomSession->getRoom()->getMap());

	const auto npcResult = getNpcsStatement->fetch();

	m_npcs.clear();
	m_spawnedNpcs.clear();

	while (npcResult->hasNext())
	{
		const auto npcId = npcResult->getInt("Id");

		const auto getLootDropsStatement = DATABASE->prepare("SELECT * FROM pve_npc_loot_drops WHERE npc_id = ?");

		getLootDropsStatement->bindInteger(npcId);

		const auto lootDropResult = getLootDropsStatement->fetch();

		std::vector<NpcLootDrop> lootDrops{};

		while (lootDropResult->hasNext())
		{
			auto lootDrop = NpcLootDrop
			{
				lootDropResult->getInt("item_id"),
				lootDropResult->getInt("probability")
			};

			lootDrops.push_back(lootDrop);

			lootDropResult->next();
		}

		const auto getBodyPartsStatement = DATABASE->prepare("SELECT * FROM pve_npc_body_parts WHERE npc_id = ?");

		getBodyPartsStatement->bindInteger(npcId);

		const auto bodyPartResult = getBodyPartsStatement->fetch();

		std::vector<NpcBodyPart> bodyParts{};

		while (bodyPartResult->hasNext())
		{
			auto bodyPart = NpcBodyPart
			{
				bodyPartResult->getInt("body_part_id"),
				bodyPartResult->getShort("health"),
				bodyPartResult->getInt("weapon_item_id"),
				bodyPartResult->getInt("item_box_id"),
				bodyPartResult->getFlag("is_dual_gun")
			};

			bodyParts.push_back(bodyPart);

			bodyPartResult->next();
		}

		auto spawnPosition = Position{
			npcResult->getFloat("SpawnPositionX"),
			npcResult->getFloat("SpawnPositionY"),
			npcResult->getFloat("SpawnPositionZ"),
		};

		auto pathFinder = spawnPosition.y < 0 ? roomSession->getUnderGroundPathfinder() : roomSession->getAboveGroundPathfinder();
		PathfinderCell spawnCell = { pathFinder->getCellX(spawnPosition.x),  pathFinder->getCellZ(spawnPosition.z) };

		auto npc = PveNpc(
			npcResult->getTiny("Type"),
			npcResult->getShort("BaseHealth"),
			npcResult->getFloat("Speed"),
			npcResult->getInt("WeaponItemId"),
			npcResult->getTiny("WeaponBodyPartId"),
			npcResult->getInt("AITime"),
			npcResult->getFloat("AttackWidth"),
			npcResult->getFloat("AttackHeight"),
			npcResult->getInt("ShouldRespawn"),
			npcResult->getInt("RespawnTime"),
			npcResult->getFlag("CanDropLoot"),
			npcResult->getShort("InitialRotation"),
			spawnPosition,
			static_cast<eNpcGradeType>(npcResult->getTiny("GradeType")),
			static_cast<eNpcMovementType>(npcResult->getTiny("MovementType")),
			static_cast<eNpcTargetType>(npcResult->getTiny("TargetType")),
			lootDrops,
			bodyParts,
			spawnCell
			);

		m_npcs.push_back(npc);

		npcResult->next();
	}
}

void RoomSessionNpcManager::spawnInitializedNpcs()
{
	for (const auto& npc : m_npcs)
	{
		spawnNpc(std::make_shared<PveNpc>(npc));
	}
}

uint32_t RoomSessionNpcManager::spawnNpc(const std::shared_ptr<PveNpc>& npc)
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return 0;
	}

	npc->setUid(m_spawnedNpcs.size() + 1);
	npc->spawn(roomSession);

	m_spawnedNpcs[npc->getUid()] = npc;

	return npc->getUid();
}

void RoomSessionNpcManager::respawnNpcByUid(const uint32_t uid)
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	const auto& npc = findNpcByUid(uid);

	if (npc == nullptr)
	{
		return;
	}

	npc->respawn(roomSession);
}

void RoomSessionNpcManager::killNpc(const uint32_t uid)
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	const auto& npc = findNpcByUid(uid);

	if (npc == nullptr)
	{
		return;
	}

	npc->die(roomSession);
}

void RoomSessionNpcManager::removeAll()
{
	m_npcs.clear();
	m_spawnedNpcs.clear();
}

std::shared_ptr<PveNpc> RoomSessionNpcManager::findNpcByUid(const uint32_t uid)
{
	const auto& it = m_spawnedNpcs.find(uid);

	if (it == m_spawnedNpcs.end())
	{
		return nullptr;
	}

	return it->second;
}

std::vector<std::shared_ptr<PveNpc>> RoomSessionNpcManager::getAliveNpcs()
{
	std::vector<std::shared_ptr<PveNpc>> aliveNpcs{};

	for (const auto& [id, npc] : m_spawnedNpcs)
	{
		if (!npc->isDead())
		{
			aliveNpcs.push_back(npc);
		}
	}

	return aliveNpcs;
}

#pragma region Event handlers

void RoomSessionNpcManager::onPlayerSync(const std::shared_ptr<RoomSessionPlayer>& session)
{
	for (const auto& npc : getAliveNpcs())
	{
		npc->spawn(session);
	}
}

void RoomSessionNpcManager::onCGPvEHitNpc(const CGPvEHitNpcData& data)
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	const auto playerId = data.roomSessionPlayer->getPlayer()->getId();
	const auto targetNpcUid = data.targetNpc->getUid();

	const auto damageDealt = data.targetNpc->takeDamage(data.weaponUsed.damage);
	const auto hasTargetDied = data.targetNpc->isDead();

	if (hasTargetDied)
	{
		killNpc(targetNpcUid);

		// TODO: Relay the kill combo.
	}

	const auto gcPvEHitNpcData = GCPvEHitNpcData
	{
		playerId,
		targetNpcUid,
		data.unk_03,
		data.impactPos,
		data.impactPosOffset,
		data.entityId,
		data.unk_11,
		data.bodyPartId,
		data.weaponUsed.itemId,
		data.weaponCardId,
		data.weaponType,
		data.hitLocation,
		data.unk_18,
		data.unk_19,
		damageDealt,
		hasTargetDied,
		0
	};

	roomSession->relayPlaying<GCPvEHitNpc>(gcPvEHitNpcData);
}

#pragma endregion