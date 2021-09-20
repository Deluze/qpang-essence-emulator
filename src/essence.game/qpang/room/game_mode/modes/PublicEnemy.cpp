#include "PublicEnemy.h"

#include "qpang/Game.h"
#include "qpang/spawn/SpawnManager.h"
#include "qpang/room/session/RoomSession.h"
#include "qpang/room/session/player/RoomSessionPlayer.h"

#include "qpang/room/tnl/net_events/server/gc_respawn.hpp"
#include "qpang/room/tnl/net_events/server/gc_game_state.hpp"
#include <utils/StringConverter.h>

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
	for (const auto& player : room->getPlayers())
	{
		player.second->setTeam(0);
	}

	room->setIsPointsGame(false);
	room->setScoreTime(10);

	GameMode::onApply(room);
}

void PublicEnemy::tick(std::shared_ptr<RoomSession> roomSession)
{
	if (roomSession->isTagSelected())
	{
		const auto currentlySelectedTag = roomSession->getCurrentlySelectedTag();
		const auto currentlySelectedTagPlayer = roomSession->find(currentlySelectedTag);

		// Check if the player is nullptr.
		if (currentlySelectedTagPlayer == nullptr)
		{
			return;
		}

		// Every tick where the tag is selected, increase its tag time.
		currentlySelectedTagPlayer->addTimeAliveAsTag();

		// Every 4th tick, take 10 health from the tag as long as they're above 100 health.
		if ((currentlySelectedTagPlayer->getTimeAliveAsTag() % 4 == 0) && (currentlySelectedTagPlayer->getHealth() > 100))
		{
			currentlySelectedTagPlayer->takeHealth(10, true);
		}

		return;
	}

	// If we get here, no tag is currently selected.
	if (roomSession->isSearchingForNextTag())
	{
		// In here we know the server is searching for the next tag, lets check the countdown.
		if (roomSession->hasTagCountdownEnded())
		{
			// Countdown has ended, lets select a new tag player.
			const auto hasFoundNextTag = roomSession->attemptToFindNextTag();

			if (hasFoundNextTag)
			{
				// A tag was found, we can stop the search.
				roomSession->stopSearchingForNextTag();

				// TODO: Announce a new tag was found.
			}
			else
			{
				// A tag was not found, reset the tag countdown.
				/*roomSession->initiateTagCountdown();
				roomSession->stopSearchingForNextTag();*/
			}
		}
		else
		{
			// Decrease tag countdown time.
			roomSession->decreaseTagCountdown();

			return;
		}
	}
	else
	{
		// Start finding next tag and initiate countdown.
		const auto canStartSearchingForNextTag = roomSession->hasInitialWaitTimeElapsed();

		if (canStartSearchingForNextTag)
		{
			const auto allPlayers = roomSession->getPlayers();

			// Iterate over all players and notify them of the selection process.
			for (const auto& player : allPlayers)
			{
				const auto playerId = player->getPlayer()->getId();

				roomSession->initiateTagCountdown();
				roomSession->initiateSearchForNextTag();

				// Send the countdown time gamestate to the player.
				player->send<GCGameState>(playerId, PUBLIC_ENEMY_START_COUNTDOWN, roomSession->getTagCountdown());
				// Initiate countdown on player screen.
				player->send<GCGameState>(playerId, PUBLIC_ENEMY_SHOW_COUNTDOWN);
			}
		}
		else
		{
			// Decrease initial wait time.
			roomSession->decreaseInitialWaitTime();

			return;
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

	roomSession->relayPlayingExcept<GCGameState>(currentlySelectedTag, currentlySelectedTag, PUBLIC_ENEMY_START_TRANSFORMATION, currentlySelectedTagPlayer->getHealth());
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

	// If the killer is the public enemy and the target is not.
	if (killerIsTagged && !targetIsTagged)
	{
		killer->addPlayerKillAsTag();
		target->addDeathByTag();
	}
	// If the target is the public enemy and the killer is not.
	else if (targetIsTagged && !killerIsTagged)
	{
		killer->addTagKillAsPlayer();
		target->addDeathAsTag();

		broadcastTagKill(roomSession, killerPlayer, isSuicide);

		target->getRoomSession()->resetCurrentlySelectedTag();
	}
	// If the target is the public enemy and they have comit suicide.
	else if (targetIsTagged && isSuicide)
	{
		broadcastTagKill(roomSession, killerPlayer, isSuicide);

		target->addDeathAsTag();
		target->getRoomSession()->resetCurrentlySelectedTag();
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