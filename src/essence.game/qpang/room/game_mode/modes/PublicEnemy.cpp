#include "PublicEnemy.h"

#include "qpang/Game.h"
#include "qpang/spawn/SpawnManager.h"
#include "qpang/room/session/RoomSession.h"
#include "qpang/room/session/player/RoomSessionPlayer.h"

#include "qpang/room/tnl/net_events/server/gc_respawn.hpp"
#include "qpang/room/tnl/net_events/server/gc_game_state.hpp"

constexpr auto PUBLIC_ENEMY_COUNTDOWN_TIME_IN_MILLIS = 5000; // Time in milliseconds.

enum CMD : uint32_t {
	PUBLIC_ENEMY_SHOW_COUNTDOWN = 31,
	PUBLIC_ENEMY_SEND_TIME = 32,
	PUBLIC_ENEMY_SEND_TRANSFORM = 36
};

bool PublicEnemy::isMissionMode()
{
	return true;
}

bool PublicEnemy::isTeamMode()
{
	return false;
}

bool PublicEnemy::isPublicEnemyMode()
{
	return true;
}

void PublicEnemy::onApply(std::shared_ptr<Room> room)
{
	GameMode::onApply(room);
}

void PublicEnemy::tick(std::shared_ptr<RoomSession> roomSession)
{
	const auto currentPublicEnemy = roomSession->getPublicEnemyPlayerId();

	// There is a public enemy, ignore the rest.
	if (currentPublicEnemy != 0)
	{
		return;
	}

	// There is no public enemy.
	if (roomSession->isSearchingForPublicEnemy())
	{
		roomSession->decreasePublicEnemyCountdownTime(1000);

		if (roomSession->getPublicEnemyCountdownTime() == 0)
		{
			roomSession->pickRandomPublicEnemy();
			// Countdown time is over, we can now set the public enemy.
			roomSession->setIsSearchingForPublicEnemy(false);
		}
	}
	else
	{
		const auto initialCountdownWaitTime = roomSession->getPublicEnemyInitialCountdownWaitTime();

		if (initialCountdownWaitTime != 0) 
		{
			roomSession->decreasePublicEnemyInitialCountdownWaitTime(1000);

			return;
		}

		// Public enemy is not being found, lets initiate it.
		for (const auto& player : roomSession->getPlayers())
		{
			const auto playerId = player->getPlayer()->getId();
			
			const auto initialCountdownWaitTime = roomSession->getPublicEnemyInitialCountdownWaitTime();

			roomSession->setPublicEnemyCountdownTime(PUBLIC_ENEMY_COUNTDOWN_TIME_IN_MILLIS);
			roomSession->setIsSearchingForPublicEnemy(true);

			player->send<GCGameState>(playerId, PUBLIC_ENEMY_SEND_TIME, PUBLIC_ENEMY_COUNTDOWN_TIME_IN_MILLIS);
			player->send<GCGameState>(playerId, PUBLIC_ENEMY_SHOW_COUNTDOWN);
		}
	}
}

void PublicEnemy::onPlayerSync(std::shared_ptr<RoomSessionPlayer> sessionPlayer)
{
	std::cout << "Synchronizing player " << sessionPlayer->getPlayer()->getId() << ".\n";

	const auto roomSession = sessionPlayer->getRoomSession();

	if (roomSession == nullptr)
	{
		return;
	}

	const auto publicEnemyId = roomSession->getPublicEnemyPlayerId();

	if (publicEnemyId == 0) 
	{
		return;
	}

	const auto publicEnemyPlayer = roomSession->find(publicEnemyId);

	if (publicEnemyPlayer == nullptr) 
	{
		return;
	}

	sessionPlayer->send<GCGameState>(publicEnemyId, PUBLIC_ENEMY_SEND_TRANSFORM, publicEnemyPlayer->getHealth());
}

void PublicEnemy::onPlayerKill(std::shared_ptr<RoomSessionPlayer> killer, std::shared_ptr<RoomSessionPlayer> target, const Weapon& weapon, uint8_t hitLocation)
{
	const auto roomSession = killer->getRoomSession();

	const auto killerPlayerId = killer->getPlayer()->getId();
	const auto targetPlayerId = target->getPlayer()->getId();

	const auto publicEnemyPlayerId = roomSession->getPublicEnemyPlayerId();

	// Basically, if the target player is the public enemy.
	if (targetPlayerId == publicEnemyPlayerId)
	{
		target->getRoomSession()->resetPublicEnemy();
		killer->addPublicEnemyScore(1);

	}

	GameMode::onPlayerKill(killer, target, weapon, hitLocation);
}