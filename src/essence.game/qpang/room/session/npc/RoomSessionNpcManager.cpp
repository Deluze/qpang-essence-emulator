#include "RoomSessionNpcManager.h"

#include "GCPvEHitNpcData.h"
#include "gc_pve_die_npc.hpp"
#include "gc_pve_hit_npc.hpp"
#include "gc_pve_npc_init.hpp"
#include "PveNpc.h"
#include "RoomSession.h"
#include "RoomSessionPlayer.h"

void RoomSessionNpcManager::initialize(const std::shared_ptr<RoomSession>& roomSession)
{
	m_roomSession = roomSession;

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

	const auto statement = DATABASE->prepare(
		"SELECT "
		"pve_npcs.id AS Id "
		",pve_npcs.type AS Type "
		",pve_npcs.base_health AS BaseHealth "
		",pve_npcs.weapon_item_id AS WeaponItemId "
		",pve_npcs.weapon_body_part_id AS WeaponBodyPartId "
		",pve_npcs.attack_time_millis AS AttackTimeInMillis "
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

	statement->bindInteger(roomSession->getRoom()->getMap());

	const auto result = statement->fetch();

	m_npcs.clear();
	m_spawnedNpcs.clear();

	while (result->hasNext())
	{
		// Primary key
		result->getInt("Id");

		// TODO: Get the drops and bodyparts.

		auto npc = PveNpc(
			result->getTiny("Type"),
			result->getShort("BaseHealth"),
			result->getInt("WeaponItemId"),
			result->getTiny("WeaponBodyPartId"),
			result->getInt("AttackTimeInMillis"),
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
			static_cast<eNpcGradeType>(result->getTiny("MovementType")),
			static_cast<eNpcMovementType>(result->getTiny("GradeType")),
			static_cast<eNpcTargetType>(result->getTiny("TargetType")));

		m_npcs.push_back(npc);

		result->next();
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
	}

	const auto gcPvEHitNpcData = GCPvEHitNpcData
	{
		playerId,
		targetNpcUid,
		data.unk_03,
		data.impactPos,
		data.impactPosOffset,
		data.unk_10,
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