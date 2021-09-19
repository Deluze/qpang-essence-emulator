#include "PublicEnemy.h"

#include "qpang/Game.h"
#include "qpang/spawn/SpawnManager.h"
#include "qpang/room/session/RoomSession.h"
#include "qpang/room/session/player/RoomSessionPlayer.h"

#include "qpang/room/tnl/net_events/server/gc_respawn.hpp"
#include "qpang/room/tnl/net_events/server/gc_game_state.hpp"
#include <utils/StringConverter.h>

constexpr auto PUBLIC_ENEMY_COUNTDOWN_TIME_IN_MILLIS = 5000; // Time in milliseconds.

enum CMD : uint32_t {
	PUBLIC_ENEMY_SHOW_COUNTDOWN = 31,
	PUBLIC_ENEMY_START_COUNTDOWN = 32,
	PUBLIC_ENEMY_UNK_01 = 33,
	PUBLIC_ENEMY_UNK_02 = 34,
	PUBLIC_ENEMY_IS_POSSIBLE = 35,
	PUBLIC_ENEMY_START_TRANSFORMATION = 36,
	PUBLIC_ENEMY_TRANSFORMATION_FINISHED = 37,
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
	room->setIsPointsGame(false);
	room->setScoreTime(10);

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
		if (roomSession->getPublicEnemyCountdownTime() == PUBLIC_ENEMY_COUNTDOWN_TIME_IN_MILLIS)
		{
			for (const auto& player : roomSession->getPlayers())
			{
				player->getPlayer()->broadcast(u"Looking for the next public enemy...");
			}
		}

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
		for (const auto& player : roomSession->getPlayingPlayers())
		{
			const auto playerId = player->getPlayer()->getId();
			
			const auto initialCountdownWaitTime = roomSession->getPublicEnemyInitialCountdownWaitTime();

			roomSession->setPublicEnemyCountdownTime(PUBLIC_ENEMY_COUNTDOWN_TIME_IN_MILLIS);
			roomSession->setIsSearchingForPublicEnemy(true);

			player->send<GCGameState>(playerId, PUBLIC_ENEMY_START_COUNTDOWN, PUBLIC_ENEMY_COUNTDOWN_TIME_IN_MILLIS);
			player->send<GCGameState>(playerId, PUBLIC_ENEMY_SHOW_COUNTDOWN);
		}
	}
}

void PublicEnemy::onPlayerSync(std::shared_ptr<RoomSessionPlayer> sessionPlayer)
{
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

	const auto position = publicEnemyPlayer->getPosition();

	sessionPlayer->post(new GCRespawn(publicEnemyId, publicEnemyPlayer->getCharacter(), 0, position.x, position.y, position.z, false));
	sessionPlayer->post(new GCGameState(publicEnemyId, PUBLIC_ENEMY_START_TRANSFORMATION, publicEnemyPlayer->getHealth()));
}

void PublicEnemy::onPlayerKill(std::shared_ptr<RoomSessionPlayer> killer, std::shared_ptr<RoomSessionPlayer> target, const Weapon& weapon, uint8_t hitLocation)
{
	const auto roomSession = killer->getRoomSession();

	const auto killerPlayer = killer->getPlayer();
	const auto targetPlayer = target->getPlayer();

	const auto tagPlayerId = roomSession->getPublicEnemyPlayerId();

	const auto isSuicide = (killerPlayer->getId() == targetPlayer->getId());

	const auto killerIsPublicEnemy = (killerPlayer->getId() == tagPlayerId);
	const auto targetIsPublicEnemy = (targetPlayer->getId() == tagPlayerId);

	if (killerIsPublicEnemy)
	{
		killer->addPlayerKillAsTag();
		target->addDeathByTag();
	}
	else if (targetIsPublicEnemy)
	{
		killer->addTagKillAsPlayer();
		target->addDeathAsTag();

		char buffer[70];

		sprintf_s(buffer, "%ls has killed the public enemy.", killerPlayer->getName().c_str());

		const auto players = roomSession->getPlayers();

		for (const auto& roomSessionPlayer : players)
		{
			const auto player = roomSessionPlayer->getPlayer();

			if ((player->getId() == killerPlayer->getId()) && !isSuicide)
			{
				player->broadcast(u"You have killed the public enemy.");
			}
			else if (!isSuicide)
			{
				player->broadcast(StringConverter::Utf8ToUtf16(buffer));
			}
			else
			{
				player->broadcast(u"The public enemy has died by suicide.");
			}
		}

		target->getRoomSession()->resetPublicEnemy();
	}
	else if (isSuicide)
	{
		target->addDeathAsTag();
		target->getRoomSession()->resetPublicEnemy();
	}

	GameMode::onPlayerKill(killer, target, weapon, hitLocation);
}