#include "PveBossNpc.h"

#include <utility>
#include <qpang/room/tnl/net_events/event_data/CGPvEHitNpcData.h>
#include <qpang/room/tnl/net_events/server/gc_pve_npc_init.hpp>

#include "gc_master_log.hpp"
#include "RandomHelper.h"

#include <Maps.h>
#include "RoomSession.h"

PveBossNpc::PveBossNpc(PveNpcData data, const PathfinderCell& spawnCell) :
	PveNpc(std::move(data), spawnCell),
	m_isInSpecialAttack(false),
	m_specialAttackDamageTreshold(1000),
	m_previousSpecialAttackHealth(data.baseHealth)
{
}

void PveBossNpc::tick(const std::shared_ptr<RoomSession>& roomSession)
{
	PveNpc::tick(roomSession);

	handleSpecialAttackStart(roomSession);
	handlePerformSpecialAttackShoot(roomSession);
	handleSpecialAttackEnd(roomSession);

	handleSpawnGoldCoins(roomSession);
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

void PveBossNpc::onDeath(const std::shared_ptr<RoomSession>& roomSession)
{
	PveNpc::onDeath(roomSession);

	m_shouldSpawnGoldCoins = true;

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

void PveBossNpc::handleSpecialAttackStart(const std::shared_ptr<RoomSession>& roomSession)
{
	if (canStartSpecialAttack())
	{
		startSpecialAttack(roomSession);
	}
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

void PveBossNpc::handleSpawnGoldCoins(const std::shared_ptr<RoomSession>& roomSession)
{
	if (m_shouldSpawnGoldCoins)
	{
		m_shouldSpawnGoldCoins = false;

		spawnGoldCoins(roomSession);
	}
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

void PveBossNpc::performSpecialAttackShoot(const std::shared_ptr<RoomSession>& roomSession) const
{
	/*for (uint32_t i = 0; i < 5; i++)
	{*/
	const auto randomX = RandomHelper::getRandomFloat(-21, 21);
	const auto randomZ = RandomHelper::getRandomFloat(-21, 21);

	const auto targetPosition = Position{ randomX, 0, randomZ };

	roomSession->relayPlaying<GCMasterLog>(m_uid, 88, targetPosition, 0);
	/*}*/
}

void PveBossNpc::spawnGoldCoins(const std::shared_ptr<RoomSession>& roomSession) const
{
	const auto cellWidth = Maps::pveStage3MapInfo.m_cellWidth;

	const float posX = 5.0f * cellWidth;
	// ReSharper disable once CppTooWideScope
	const float posZ = 5.0f * cellWidth;

	for (float x = -posX; x <= posX; x += cellWidth * 2.0f)
	{
		for (float z = -posZ; z <= posZ; z += cellWidth * 2.0f)
		{
			const auto goldCoin = PveItem(static_cast<uint32_t>(eItemId::GOLDEN_COIN), Position{ x, 0, z });

			roomSession->getPveItemManager()->spawnItem(std::make_shared<PveItem>(goldCoin));
		}
	}
}