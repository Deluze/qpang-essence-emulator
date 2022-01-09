#include "PveBossNpc.h"

#include "RoomSession.h"

#include <utility>
#include <qpang/room/tnl/net_events/event_data/CGPvEHitNpcData.h>
#include <qpang/room/tnl/net_events/server/gc_pve_npc_init.hpp>

#include "gc_master_log.hpp"

PveBossNpc::PveBossNpc(PveNpcData data, const PathfinderCell& spawnCell) :
	PveNpc(std::move(data), spawnCell),
	m_isInSpecialAttack(false),
	m_specialAttackDamageTreshold(5000),
	m_previousSpecialAttackHealth(data.baseHealth)
{
}

void PveBossNpc::tick(const std::shared_ptr<RoomSession>& roomSession)
{
	PveNpc::tick(roomSession);

	if (canStartSpecialAttack())
	{
		startSpecialAttack(roomSession);

		return;
	}

	const auto currentTime = time(nullptr);

	if (m_lastSpecialAttackTime == NULL)
	{
		return;
	}

	if ((m_lastSpecialAttackTime + m_specialAttackDuration) < currentTime)
	{
		endSpecialAttack(roomSession);
	}
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

bool PveBossNpc::canStartSpecialAttack() const
{
	return (!m_isInSpecialAttack && (m_health < (m_previousSpecialAttackHealth - m_specialAttackDamageTreshold)));
}

void PveBossNpc::startSpecialAttack(const std::shared_ptr<RoomSession>& roomSession)
{
	m_isInSpecialAttack = true;
	m_previousSpecialAttackHealth = m_health;

	m_lastSpecialAttackTime = time(nullptr);

	roomSession->relayPlaying<GCMasterLog>(m_uid, 0, 0);
}

void PveBossNpc::endSpecialAttack(const std::shared_ptr<RoomSession>& roomSession)
{
	m_isInSpecialAttack = false;
	m_lastSpecialAttackTime = NULL;

	roomSession->relayPlaying<GCMasterLog>(m_uid, 0, 1);
}
