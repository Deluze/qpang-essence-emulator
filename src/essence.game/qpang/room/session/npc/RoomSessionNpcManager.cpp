#include "RoomSessionNpcManager.h"

#include "Position.h"
#include "RoomSession.h"
#include "RoomSessionPlayer.h"

#include <vector>

#include "gc_pve_die_npc.hpp"
#include "gc_pve_npc_init.hpp"

std::vector<uint32_t> npcs{};

void RoomSessionNpcManager::initialize(const std::shared_ptr<RoomSession>& roomSession)
{
	m_roomSession = roomSession;
}

uint32_t RoomSessionNpcManager::spawnNpc(const Npc npc)
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return 0;
	}

	const auto npcUid = (m_npcs.size() + 1);

	roomSession->relayPlaying<GCPvENpcInit>(npc.type, npcUid, npc.position.x, npc.position.y, npc.position.z);

	m_npcs[npcUid] = npc;
	return npcUid;
}

void RoomSessionNpcManager::killNpcByUid(const uint32_t uid)
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	if (const auto npc = findNpcByUid(uid); npc == nullptr)
	{
		return;
	}

	roomSession->relayPlaying<GCPvEDieNpc>(uid);

	m_npcs.erase(uid);
}

RoomSessionNpcManager::Npc* RoomSessionNpcManager::findNpcByUid(const uint32_t uid)
{
	const auto it = m_npcs.find(uid);

	if (it == m_npcs.end())
	{
		return nullptr;
	}

	return &it->second;
}

void RoomSessionNpcManager::onPlayerSync(std::shared_ptr<RoomSessionPlayer> session)
{
	for (auto& npc : m_npcs)
	{
		session->send<GCPvENpcInit>(npc.second.type, npc.first, npc.second.position.x, npc.second.position.y, npc.second.position.z);
	}
}

void RoomSessionNpcManager::tick()
{

}