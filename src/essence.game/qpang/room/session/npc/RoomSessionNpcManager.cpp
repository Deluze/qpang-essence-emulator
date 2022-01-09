// ReSharper disable CppClangTidyBugproneBranchClone
#include "RoomSessionNpcManager.h"

#include "GCPvEHitNpcData.h"
#include "gc_pve_die_npc.hpp"
#include "gc_pve_hit_npc.hpp"
#include "PveBossNpc.h"
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

void RoomSessionNpcManager::tick()
{
	std::lock_guard<std::recursive_mutex> lg(m_npcMutex);

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
	std::lock_guard<std::recursive_mutex> lg(m_npcMutex);

	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	m_spawnedNpcs.clear();
	m_npcs.clear();

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

		const bool isBossNpc = data.type == 13 || data.type == 38 || data.type == 39 || data.type == 59;
		const auto npc = isBossNpc ? std::make_shared<PveBossNpc>(data, spawnCell) :
			std::make_shared<PveNpc>(data, spawnCell);

		m_npcs.push_back(npc);
	}
}

void RoomSessionNpcManager::spawnNpcsForArea(const uint32_t areaId)
{
	std::lock_guard<std::recursive_mutex> lg(m_npcMutex);

	for (const auto& npc : m_npcs)
	{
		if (npc->getAreaUid() == areaId)
		{
			spawnNpc(npc);
		}
	}
}

uint32_t RoomSessionNpcManager::spawnNpc(const std::shared_ptr<PveNpc>& npc)
{
	std::lock_guard<std::recursive_mutex> lg(m_npcMutex);

	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return 0;
	}

	if (npc->getUid() == 0)
	{
		// ReSharper disable once CppTooWideScopeInitStatement
		const auto nextUid = (m_lastSpawnedUid + 1);

		if (m_spawnedNpcs.count(nextUid) == 0)
		{
			npc->setUid(nextUid);
		}
		else
		{
			m_lastSpawnedUid = nextUid;

			spawnNpc(npc);

			return 0;
		}
	}

	npc->spawn(roomSession);

	m_spawnedNpcs[npc->getUid()] = npc;

	m_lastSpawnedUid = npc->getUid();

	return npc->getUid();
}

void RoomSessionNpcManager::respawnNpcByUid(const uint32_t uid)
{
	std::lock_guard<std::recursive_mutex> lg(m_npcMutex);

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
	std::lock_guard<std::recursive_mutex> lg(m_npcMutex);

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
	std::lock_guard<std::recursive_mutex> lg(m_npcMutex);

	m_npcs.clear();
	m_spawnedNpcs.clear();
}

std::shared_ptr<PveNpc> RoomSessionNpcManager::findNpcByUid(const uint32_t uid)
{
	std::lock_guard<std::recursive_mutex> lg(m_npcMutex);

	const auto& it = m_spawnedNpcs.find(uid);

	if (it == m_spawnedNpcs.end())
	{
		return nullptr;
	}

	return it->second;
}

std::vector<std::shared_ptr<PveNpc>> RoomSessionNpcManager::getAliveNpcs()
{
	std::lock_guard<std::recursive_mutex> lg(m_npcMutex);

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
	std::lock_guard<std::recursive_mutex> lg(m_npcMutex);

	for (const auto& npc : getAliveNpcs())
	{
		npc->spawn(session);
	}
}

void RoomSessionNpcManager::onCGPvEHitNpc(const CGPvEHitNpcData& data)
{
	std::lock_guard<std::recursive_mutex> lg(m_npcMutex);

	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	const auto playerId = data.roomSessionPlayer->getPlayer()->getId();
	const auto targetNpcUid = data.targetNpc->getUid();

	const auto& targetNpc = findNpcByUid(targetNpcUid);

	if (targetNpc == nullptr)
	{
		return;
	}

	const auto damage = targetNpc->calculateHitDamage(data);

	const auto damageDealtToBodyPart = targetNpc->takeBodyPartDamage(data.bodyPartId, damage);
	const auto damageDealtToNpc = targetNpc->takeDamage(static_cast<uint16_t>(damage));

	const auto hasTargetDied = targetNpc->isDead();

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
		static_cast<uint8_t>(data.bodyPartId),
		data.weaponUsed.itemId,
		data.weaponCardId,
		data.weaponType,
		data.hitLocation,
		data.unk_18,
		data.unk_19,
		damageDealtToNpc,
		killStreak,
		targetNpc->getHealth()
	};

	roomSession->relayPlaying<GCPvEHitNpc>(gcPvEHitNpcData);
}

#pragma endregion