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

	spawnInitialNpcs();
}

void RoomSessionNpcManager::tick() const
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	for (const auto& [uid, npc] : m_npcs)
	{
		npc->tick(roomSession);
	}
}

void RoomSessionNpcManager::spawnInitialNpcs()
{
	// Stage 1.
	const std::vector npcs
	{
		// Wall 1 left, first spy cam.
		std::make_shared<PveNpc>(eNpcType::EASY_SPY_CAM, Position{-11.99f, 3.12f, -20.30f}, 60, 90, false, true, 5),
		// Wall 1 left, last spy cam.
		std::make_shared<PveNpc>(eNpcType::EASY_SPY_CAM, Position{-3.91f, 3.12f, -20.30f}, 60, 90, false, true, 5),

		// Wall 2 left, first spy cam.
		std::make_shared<PveNpc>(eNpcType::EASY_SPY_CAM, Position{17.70f, 3.155f, -20.0f}, 60, 90, false, true, 5),
		// Wall 2 right, first middle spy cam.
		std::make_shared<PveNpc>(eNpcType::EASY_SPY_CAM, Position{23.65f, 3.155f, -26.80f}, 60, 270, false, true, 5),
		// Wall 2 left, last spy cam.
		std::make_shared<PveNpc>(eNpcType::EASY_SPY_CAM, Position{33.30f, 3.155f, -20.0f}, 60, 90, false, true, 5),

		// Wall 3 right, first spy cam.
		std::make_shared<PveNpc>(eNpcType::EASY_SPY_CAM, Position{25.04f, 3.10f, 9.85f}, 60, 270, false, true, 5),
		// Wall 3 right, last spy cam.
		std::make_shared<PveNpc>(eNpcType::EASY_SPY_CAM, Position{32.37f, 3.15f, 8.40f}, 60, 270, false, true, 5),

		// First violent plant.
		std::make_shared<PveNpc>(eNpcType::EASY_VIOLENT_PLANT, Position{-3.62f, 0.00f, -32.86f }, 60, 270, true, true, 5),
	};

	for (auto& npc : npcs)
	{
		spawnNpc(npc);
	}
}

uint32_t RoomSessionNpcManager::spawnNpc(const std::shared_ptr<PveNpc>& npc)
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return 0;
	}

	const auto npcUid = (m_npcs.size() + 1);

	npc->setUid(npcUid);

	roomSession->relayPlaying<GCPvENpcInit>(npc->getType(), npcUid, npc->getInitialSpawnPosition(), npc->getInitialSpawnRotation());

	m_npcs[npcUid] = npc;

	return npcUid;
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

	npc->resetHealth();

	roomSession->relayPlaying<GCPvENpcInit>(npc->getType(), npc->getUid(), npc->getInitialSpawnPosition(), npc->getInitialSpawnRotation());
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

	roomSession->relayPlaying<GCPvEDieNpc>(uid);

	npc->onDeath(roomSession);
}

void RoomSessionNpcManager::removeAll()
{
	m_npcs.clear();
}

std::shared_ptr<PveNpc> RoomSessionNpcManager::findNpcByUid(const uint32_t uid)
{
	const auto& it = m_npcs.find(uid);

	if (it == m_npcs.end())
	{
		return nullptr;
	}

	return it->second;
}

std::vector<std::shared_ptr<PveNpc>> RoomSessionNpcManager::getAliveNpcs()
{
	std::vector<std::shared_ptr<PveNpc>> aliveNpcs{};

	for (const auto& [id, npc] : m_npcs)
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
		session->send<GCPvENpcInit>(npc->getType(), npc->getUid(), npc->getPosition(), npc->getInitialSpawnRotation());
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