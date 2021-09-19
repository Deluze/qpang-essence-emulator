#include "PublicEnemy.h"

#include "qpang/Game.h"
#include "qpang/spawn/SpawnManager.h"
#include "qpang/room/session/RoomSession.h"
#include "qpang/room/session/player/RoomSessionPlayer.h"

#include "qpang/room/tnl/net_events/server/gc_respawn.hpp"
#include "qpang/room/tnl/net_events/server/gc_game_state.hpp"
#include <utils/StringConverter.h>

constexpr auto PUBLIC_ENEMY_COUNTDOWN_TIME = 5000;

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
	const auto currentlySelectedTag = roomSession->getCurrentlySelectedTag();

	if (currentlySelectedTag != 0)
	{
		const auto currentlySelectedTagPlayer = roomSession->find(currentlySelectedTag);

		if (currentlySelectedTagPlayer == nullptr)
		{
			return;
		}

		currentlySelectedTagPlayer->takeHealth(10);

		return;
	}

	if (roomSession->isFindingNextTag())
	{
		if (roomSession->getSelectTagCountdownTime() == PUBLIC_ENEMY_COUNTDOWN_TIME)
		{
			for (const auto& player : roomSession->getPlayers())
			{
				player->getPlayer()->broadcast(u"Looking for the next public enemy...");
			}
		}

		roomSession->decreaseSelectTagCountdownTime(1000);

		if (roomSession->getSelectTagCountdownTime() == 0)
		{
			roomSession->findNextTag();
			// Countdown time is over, we can now set the public enemy.
			roomSession->setIsFindingNextTag(false);
		}
	}
	else
	{
		const auto initialWaitTime = roomSession->getSelectTagInitialWaitTime();

		if (initialWaitTime != 0)
		{
			roomSession->decreaseSelectTagInitialWaitTime(1000);

			return;
		}

		const auto allPlayers = roomSession->getPlayers();

		// Public enemy is not being found, lets initiate it.
		for (const auto& player : allPlayers)
		{
			const auto playerId = player->getPlayer()->getId();

			roomSession->setSelectTagCountdownTime(PUBLIC_ENEMY_COUNTDOWN_TIME);
			roomSession->setIsFindingNextTag(true);

			player->send<GCGameState>(playerId, PUBLIC_ENEMY_START_COUNTDOWN, PUBLIC_ENEMY_COUNTDOWN_TIME);
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

	const auto currentlySelectedTag = roomSession->getCurrentlySelectedTag();

	if (currentlySelectedTag == 0)
	{
		return;
	}

	const auto currentlySelectedTagPlayer = roomSession->find(currentlySelectedTag);

	if (currentlySelectedTagPlayer == nullptr)
	{
		return;
	}

	const auto position = currentlySelectedTagPlayer->getPosition();

	sessionPlayer->post(new GCRespawn(currentlySelectedTag, currentlySelectedTagPlayer->getCharacter(), 0, position.x, position.y, position.z, false));
	sessionPlayer->post(new GCGameState(currentlySelectedTag, PUBLIC_ENEMY_START_TRANSFORMATION, currentlySelectedTagPlayer->getHealth()));
}

void PublicEnemy::onPlayerKill(std::shared_ptr<RoomSessionPlayer> killer, std::shared_ptr<RoomSessionPlayer> target, const Weapon& weapon, uint8_t hitLocation)
{
	const auto roomSession = killer->getRoomSession();

	const auto killerPlayer = killer->getPlayer();
	const auto targetPlayer = target->getPlayer();

	const auto tagPlayerId = roomSession->getCurrentlySelectedTag();

	const auto isSuicide = (killerPlayer->getId() == targetPlayer->getId());

	const auto killerIsTagged = (killerPlayer->getId() == tagPlayerId);
	const auto targetIsTagged = (targetPlayer->getId() == tagPlayerId);

	if (killerIsTagged)
	{
		killer->addPlayerKillAsTag();
		target->addDeathByTag();
	}
	else if (targetIsTagged)
	{
		killer->addTagKillAsPlayer();
		target->addDeathAsTag();

		broadcastTagKill(roomSession, killerPlayer, isSuicide);

		target->getRoomSession()->clearCurrentTag();
	}
	else if (isSuicide)
	{
		target->addDeathAsTag();
		target->getRoomSession()->clearCurrentTag();
	}

	GameMode::onPlayerKill(killer, target, weapon, hitLocation);
}

void PublicEnemy::broadcastTagKill(RoomSession::Ptr roomSession, Player::Ptr killer, bool isSuicide)
{
	char buffer[70];

	sprintf_s(buffer, "%ls has killed the public enemy.", killer->getName().c_str());

	const auto players = roomSession->getPlayers();

	for (const auto& roomSessionPlayer : players)
	{
		const auto player = roomSessionPlayer->getPlayer();

		if ((player->getId() == killer->getId()) && !isSuicide)
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
}