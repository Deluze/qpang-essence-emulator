#include "RoomSessionNpcManager.h"

#include "RoomSession.h"
#include "RoomSessionPlayer.h"

#include "gc_pve_die_npc.hpp"
#include "gc_pve_npc_init.hpp"

#include "PveNpc.h"

void RoomSessionNpcManager::initialize(const std::shared_ptr<RoomSession>& roomSession)
{
	m_roomSession = roomSession;

	spawnInitialNpcs();
}

void RoomSessionNpcManager::spawnInitialNpcs()
{
	// Stage 1.
	const std::vector npcs
	{
		PveNpc(eNpcType::VIOLENT_RABBIT, Position{23.8f, 0.02f, -36.36f}),
		PveNpc(eNpcType::NASTY_RABBIT, Position{18.69f, 0.02f, -36.36f}),
		PveNpc(eNpcType::CRAZY_RABBIT, Position{18.69f, 0.02f, -33.85f}),
		PveNpc(eNpcType::BLACK_CAT, Position{22.57f, 0.02f, -33.22f}),
	};

	for (const auto& npc : npcs)
	{
		spawnNpc(npc);
	}
}

uint32_t RoomSessionNpcManager::spawnNpc(PveNpc npc)
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return 0;
	}

	npc.setUid(m_npcs.size() + 1);

	roomSession->relayPlaying<GCPvENpcInit>(npc.getType(), npc.getUid(), npc.getPosition());

	m_npcs[npc.getUid()] = npc;

	return npc.getUid();
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

PveNpc* RoomSessionNpcManager::findNpcByUid(const uint32_t uid)
{
	const auto it = m_npcs.find(uid);

	if (it == m_npcs.end())
	{
		return nullptr;
	}

	return &it->second;
}

void RoomSessionNpcManager::onPlayerSync(const std::shared_ptr<RoomSessionPlayer>& session) const
{
	for (const auto& npc : m_npcs)
	{
		session->send<GCPvENpcInit>(npc.second.getType(), npc.second.getUid(), npc.second.getPosition());
	}
}

void RoomSessionNpcManager::tick(const std::shared_ptr<RoomSession>& roomSession) const
{

}