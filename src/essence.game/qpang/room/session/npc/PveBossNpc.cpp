#include "PveBossNpc.h"

#include <Maps.h>
#include <utility>
#include <qpang/room/tnl/net_events/event_data/CGPvEHitNpcData.h>
#include <qpang/room/tnl/net_events/server/gc_pve_npc_init.hpp>

#include "gc_master_log.hpp"
#include "gc_pve_die_npc.hpp"
#include "RandomHelper.h"
#include "RoomSession.h"

PveBossNpc::PveBossNpc(PveNpcData data, const PathfinderCell& spawnCell) :
	PveNpc(std::move(data), spawnCell),
	m_specialAttackDamageTreshold(500),				// Should be total of 4 times, so m_baseHealt / 4?
	m_previousSpecialAttackHealth(data.baseHealth),
	m_specialAttackDuration(30)						// Should be 60 (full minute, like in the videos). 
{
}

void PveBossNpc::tick(const std::shared_ptr<RoomSession>& roomSession)
{
	if (canStartSpecialAttack())
	{
		startSpecialAttack(roomSession);
	}
	else
	{
		handlePerformSpecialAttackShoot(roomSession);
		handleSpecialAttackEnd(roomSession);
	}

	PveNpc::tick(roomSession);
}

float PveBossNpc::calculateHitDamage(const CGPvEHitNpcData& data)
{
	if (m_isInSpecialAttack)
	{
		return 0;
	}

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

void PveBossNpc::attackTargetPlayer(const std::shared_ptr<RoomSession>& roomSession)
{
	if (m_targetPlayerId == 0 || m_isInSpecialAttack)
	{
		return;
	}

	const auto targetPlayer = roomSession->find(m_targetPlayerId);

	if (!targetPlayer || !isPlayerValid(targetPlayer))
	{
		return;
	}

	const auto targetPosition = targetPlayer->getPosition();

	m_targetShootPosition = targetPosition;

	attackTargetPos(roomSession);
}

void PveBossNpc::startMovingToPlayer(const std::shared_ptr<RoomSession>& roomSession, Pathfinder* pathFinder)
{
	if (m_isInSpecialAttack)
	{
		return;
	}

	PveNpc::startMovingToPlayer(roomSession, pathFinder);
}

void PveBossNpc::onDeath(const std::shared_ptr<RoomSession>& roomSession)
{
	PveNpc::onDeath(roomSession);

	for (const auto& npc : roomSession->getNpcManager()->getAliveNpcs())
	{
		npc->takeDamage(npc->getHealth());
		npc->die(roomSession);
	}
}

bool PveBossNpc::canStartSpecialAttack() const
{
	return (!m_isInSpecialAttack && (m_health < (m_previousSpecialAttackHealth - m_specialAttackDamageTreshold)));
}

void PveBossNpc::handleSpecialAttackEnd(const std::shared_ptr<RoomSession>& roomSession)
{
	const auto currentTime = time(nullptr);

	if (m_isInSpecialAttack && (m_lastSpecialAttackTime != NULL) && (m_lastSpecialAttackTime + m_specialAttackDuration) < currentTime)
	{
		endSpecialAttack(roomSession);
	}
}

void PveBossNpc::handlePerformSpecialAttackShoot(const std::shared_ptr<RoomSession>& roomSession)
{
	if (m_isInSpecialAttack && canShoot())
	{
		performSpecialAttackShoot(roomSession);
	}
}

void PveBossNpc::startSpecialAttack(const std::shared_ptr<RoomSession>& roomSession)
{
	m_isInSpecialAttack = true;
	m_previousSpecialAttackHealth = m_health;

	m_lastSpecialAttackTime = time(nullptr);

	roomSession->relayPlaying<GCMasterLog>(m_uid, 0, 0);

	const std::vector<uint32_t> uids{ 1337, 1338, 1339, 1340 };

	for (const auto uid : uids)
	{
		const auto spawnPosition = Position
		{
			m_position.x,
			m_position.y + 20,
			m_position.z
		};

		roomSession->relayPlaying<GCPvENpcInit>(m_type, uid, spawnPosition, m_initialRotation, m_baseHealth);
	}
}

void PveBossNpc::endSpecialAttack(const std::shared_ptr<RoomSession>& roomSession)
{
	m_isInSpecialAttack = false;
	m_lastSpecialAttackTime = NULL;

	roomSession->relayPlaying<GCMasterLog>(m_uid, 0, 1);
}

void PveBossNpc::performSpecialAttackShoot(const std::shared_ptr<RoomSession>& roomSession) const
{
	const std::vector<uint32_t> uids{ m_uid, 1337, 1338, 1339, 1340 };

	for (const auto uid : uids)
	{
		const auto randomX = RandomHelper::getRandomFloat(-21, 21);
		const auto randomZ = RandomHelper::getRandomFloat(-21, 21);

		const auto targetPosition = Position{ randomX, 0, randomZ };

		roomSession->relayPlaying<GCMasterLog>(uid, 88, targetPosition, time(nullptr));
	}
}
