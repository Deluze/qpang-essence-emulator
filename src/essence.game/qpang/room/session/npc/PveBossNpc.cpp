#include "PveBossNpc.h"

#include "RoomSession.h"

#include <qpang/room/tnl/net_events/server/gc_pve_npc_init.hpp>
#include <utility>
#include <qpang/room/tnl/net_events/event_data/CGPvEHitNpcData.h>

PveBossNpc::PveBossNpc(PveNpcData data, const PathfinderCell& spawnCell) :
	PveNpc(std::move(data), spawnCell)
{
}

void PveBossNpc::tick(const std::shared_ptr<RoomSession>& roomSession)
{
	PveNpc::tick(roomSession);
}

float PveBossNpc::calculateHitDamage(const CGPvEHitNpcData& data)
{
	return PveNpc::calculateHitDamage(data);
}

void PveBossNpc::spawn(const std::shared_ptr<RoomSession>& roomSession) const
{
	roomSession->relayPlaying<GCPvENpcInit>(m_type, m_uid, m_initialPosition, m_initialRotation, m_health, true);
}

void PveBossNpc::spawn(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer) const
{
	roomSessionPlayer->send<GCPvENpcInit>(m_type, m_uid, m_initialPosition, m_initialRotation, m_health, true);
}