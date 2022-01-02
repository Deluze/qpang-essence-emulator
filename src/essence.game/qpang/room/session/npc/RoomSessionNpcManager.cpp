// ReSharper disable CppClangTidyBugproneBranchClone
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

	m_npcs.clear();
	m_spawnedNpcs.clear();

	const auto mapId = roomSession->getPveRoundManager()->getMap();
	const auto npcData = Game::instance()->getPveManager()->getNpcDataByMapId(mapId);

	for (auto& data : npcData)
	{
		const auto [x, y, z] = data.initialPosition;

		const auto pathFinder = (y < 0)
			? roomSession->getUnderGroundPathfinder()
			: roomSession->getAboveGroundPathfinder();

		PathfinderCell spawnCell = {
			pathFinder->getCellX(x),
			pathFinder->getCellZ(z)
		};

		const auto npc = PveNpc(data, spawnCell);

		m_npcs.push_back(npc);
	}
}

void RoomSessionNpcManager::spawnNpcsForArea(const uint32_t areaId)
{
	for (const auto& npc : m_npcs)
	{
		if (npc.getAreaUid() == areaId)
		{
			spawnNpc(std::make_shared<PveNpc>(npc));
		}
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

	auto damage = static_cast<double>(data.weaponUsed.damage);

	// Note: Currently there are no bodyparts with attribute type A_DEFAULT or A_DESTROY.
	switch (data.bodyPart.attributeType)
	{
	case eNpcBodyPartAttributeType::A_DEFAULT:
		damage = 0;
		break;
	case eNpcBodyPartAttributeType::A_INCAPACITY:
		// FIXME: If we interpret the description correctly, this body part needs to be destroyed on hit (gc_pve_destroy_part).
	case eNpcBodyPartAttributeType::A_DESTROY:
		break;
	}

	// Note: Currently the damage type D_ONLY_NEARWEAPON is not being used by any npc.
	switch (data.bodyPart.damageType)
	{
	case eNpcBodyPartDamageType::D_DEFAULT:
		// Takes damage regardless of weapon.
		break;
	case eNpcBodyPartDamageType::D_EXCEPT_SPLASH:
		// FIXME: Check if weapon deals splash damage. For now, we disable all damage from throwables.
		if (data.weaponType == eWeaponType::THROWABLE_1)
			damage = 0;

		break;
	case eNpcBodyPartDamageType::D_ONLY_NEARWEAPON:
		if (data.weaponType != eWeaponType::MELEE)
			damage = 0;

		break;
	}

	const auto npcBodyParts = data.targetNpc->getBodyParts();
	// ReSharper disable once CppTooWideScopeInitStatement
	const auto isHeadshot = (!npcBodyParts.empty() && (npcBodyParts[0].id == data.bodyPart.id));

	// This means it is headshot damage.
	if (!isHeadshot)
		damage *= 0.5;

	const auto damageDealt = data.targetNpc->takeDamage(static_cast<uint16_t>(damage));
	const auto hasTargetDied = data.targetNpc->isDead();

	if (hasTargetDied)
	{
		killNpc(targetNpcUid);

		data.roomSessionPlayer->addStreak();
	}

	const auto killStreak = hasTargetDied
		? std::clamp<uint8_t>(data.roomSessionPlayer->getStreak(), 0, 255)
		: static_cast<uint8_t>(0);

	const auto gcPvEHitNpcData = GCPvEHitNpcData
	{
		playerId,
		targetNpcUid,
		data.unk_03,
		data.impactPos,
		data.impactPosOffset,
		data.entityId,
		data.unk_11,
		static_cast<uint8_t>(data.bodyPart.id),
		data.weaponUsed.itemId,
		data.weaponCardId,
		data.weaponType,
		data.hitLocation,
		data.unk_18,
		data.unk_19,
		damageDealt,
		killStreak,
		data.targetNpc->getHealth()
	};

	roomSession->relayPlaying<GCPvEHitNpc>(gcPvEHitNpcData);
}

#pragma endregion