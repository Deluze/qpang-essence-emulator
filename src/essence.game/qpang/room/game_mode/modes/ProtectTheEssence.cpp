#include "ProtectTheEssence.h"

#include "qpang/Game.h"
#include "qpang/spawn/SpawnManager.h"
#include "qpang/room/session/RoomSession.h"
#include "qpang/room/session/player/RoomSessionPlayer.h"

#include "qpang/room/tnl/net_events/server/gc_respawn.hpp"
#include "qpang/room/tnl/net_events/server/gc_hit_essence.hpp"

#define ESSENCE_RESET_TIME 20

bool ProtectTheEssence::isMissionMode()
{
	return true;
}

bool ProtectTheEssence::isTeamMode()
{
	return true;
}

void ProtectTheEssence::onApply(std::shared_ptr<Room> room)
{
	room->setIsPointsGame(false);
	room->setScoreTime(10);
	
	GameMode::onApply(room);
}

void ProtectTheEssence::tick(std::shared_ptr<RoomSession> roomSession)
{
	auto essenceHolder = roomSession->getEssenceHolder();

	if (essenceHolder != nullptr)
		essenceHolder->addScore();

	const auto eligiblePoints = roomSession->getTimeLeftInSeconds();
	const auto bluePoints = roomSession->getBluePoints();
	const auto yellowPoints = roomSession->getYellowPoints();

	if ((bluePoints + eligiblePoints < yellowPoints) || (yellowPoints + eligiblePoints < bluePoints))
		roomSession->finish();

	if (!roomSession->isEssenceReset() && roomSession->isEssenceDropped())
		if (roomSession->getElapsedEssenceDropTime() > ESSENCE_RESET_TIME)
			roomSession->resetEssence();
}

void ProtectTheEssence::onPlayerSync(std::shared_ptr<RoomSessionPlayer> sessionPlayer)
{
	auto roomSession = sessionPlayer->getRoomSession();
	auto essenceHolder = roomSession->getEssenceHolder();

	if (essenceHolder != nullptr)
	{
		const auto& pos = roomSession->getEssencePosition();
		sessionPlayer->post(new GCRespawn(0, 3, 5));
		sessionPlayer->post(new GCHitEssence(sessionPlayer->getPlayer()->getId(), essenceHolder->getPlayer()->getId(), 2, pos.x, pos.y, pos.z, 0, 6));
	}
	else
	{
		const auto& pos = sessionPlayer->getRoomSession()->getEssencePosition();

		sessionPlayer->post(new GCRespawn(0, 3, 5, pos.x, pos.y, pos.z));
		sessionPlayer->post(new GCHitEssence(sessionPlayer->getPlayer()->getId(), sessionPlayer->getPlayer()->getId(), 3, pos.x, pos.y, pos.z, 0, 6));
	}

	GameMode::onPlayerSync(sessionPlayer);
}

void ProtectTheEssence::onPlayerKill(std::shared_ptr<RoomSessionPlayer> killer, std::shared_ptr<RoomSessionPlayer> target, const Weapon& weapon, uint8_t hitLocation)
{
	auto roomSession = killer->getRoomSession();
	auto essenceHolder = roomSession->getEssenceHolder();

	if (essenceHolder != nullptr)
	{
		const auto essenceTargetDied = target == essenceHolder;

		if (essenceTargetDied)
		{
			const auto players = roomSession->getPlayingPlayers();
			const auto& pos = essenceHolder->getPosition();

			roomSession->setEssenceHolder(nullptr);

			for (const auto& player : players)
				player->post(new GCHitEssence(player->getPlayer()->getId(), essenceHolder->getPlayer()->getId(), 3, pos.x, pos.y, pos.z, 0, 6));
		}
	}

	GameMode::onPlayerKill(killer, target, weapon, hitLocation);
}
