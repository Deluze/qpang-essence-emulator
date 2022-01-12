#include "RoomSessionPveRoundManager.h"

#include "gc_pve_new_round.hpp"
#include "gc_pve_round_end.hpp"
#include "RoomSession.h"

#include <qpang/helpers/AABBHelper.h>

#include "cg_game_state.hpp"
#include "gc_game_state.hpp"
#include "Maps.h"
#include <qpang/room/tnl/net_events/server/gc_join.hpp>

void RoomSessionPveRoundManager::initialize(const std::shared_ptr<RoomSession>& roomSession)
{
	m_roomSession = roomSession;
	m_currentMap = roomSession->getRoom()->getMap();
}

void RoomSessionPveRoundManager::onStart() const
{
	updatePathfinders();
}

void RoomSessionPveRoundManager::updatePathfinders() const
{
	const auto roomSession = m_roomSession.lock();
	if (roomSession == nullptr)
		return;

	switch (m_currentRound)
	{
	case eRound::CHESS_CASTLE_STAGE_1:
		roomSession->getAboveGroundPathfinder()->updateMapInfo(Maps::pveStage1AboveGroundMapInfo);
		roomSession->getUnderGroundPathfinder()->updateMapInfo(Maps::pveStage1UnderGroundMapInfo);
		break;
	case eRound::CHESS_CASTLE_STAGE_2:
		roomSession->getAboveGroundPathfinder()->updateMapInfo(Maps::pveStage2MapInfo);
		roomSession->getUnderGroundPathfinder()->free();
		break;
	case eRound::CHESS_CASTLE_STAGE_3:
		roomSession->getAboveGroundPathfinder()->updateMapInfo(Maps::pveStage3MapInfo);
		roomSession->getUnderGroundPathfinder()->free();
		break;
	}
}

void RoomSessionPveRoundManager::onStartNewRound(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer)
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	m_initializedPlayerCount++;

	// Check if the initialized player count equals the playing players size.
	if (m_initializedPlayerCount >= roomSession->getPlayers().size())
	{
		for (const auto& player : roomSession->getPlayers())
		{
			roomSession->addPlayer(player->getGameConnection(), player->getTeam());
		}

		// Reset the time.
		roomSession->resetTime();

		m_currentMap++;

		updatePathfinders();

		// Initialize all areas,objects, npcs and items.
		roomSession->getPveAreaManager()->initializeAreas();
		roomSession->getObjectManager()->initializeObjects();
		roomSession->getNpcManager()->initializeNpcs();
		roomSession->getPveItemManager()->initializeItems();

		if (m_currentRound == eRound::CHESS_CASTLE_STAGE_2)
		{
			roomSession->getPveWaveManager()->initializeWaves();
		}

		for (const auto& player : roomSession->getPlayers())
		{
			// Synchronize time for every player.
			player->send<GCGameState>(player->getPlayer()->getId(), CGGameState::State::SYNC_TIME, roomSession->getElapsedTime());
			//player->send<GCPvENpcInit>(1, 1337, Position{ 0,0,0 }, 0, 0);
		}

		m_hasRoundEnded = false;
	}
}

void RoomSessionPveRoundManager::endRound()
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	m_hasRoundEnded = true;

	roomSession->getPveAreaManager()->removeAll();
	roomSession->getObjectManager()->removeAll();
	roomSession->getNpcManager()->removeAll();
	roomSession->getPveItemManager()->removeAll();
	roomSession->getPveWaveManager()->removeAll();

	m_initializedPlayerCount = 0;

	roomSession->stopTime();

	if (m_currentRound == eRound::CHESS_CASTLE_STAGE_3)
	{
		roomSession->finishPveGame(true);

		return;
	}

	switch (m_currentRound)
	{
	case eRound::CHESS_CASTLE_STAGE_1:
		m_currentRound = eRound::CHESS_CASTLE_STAGE_2;
		break;
	case eRound::CHESS_CASTLE_STAGE_2:
		m_currentRound = eRound::CHESS_CASTLE_STAGE_3;
		break;
	case eRound::CHESS_CASTLE_STAGE_3:
		return;
	}

	for (const auto& player : roomSession->getPlayingPlayers())
	{
		// Synchronize time for every player.
		player->send<GCGameState>(player->getPlayer()->getId(), CGGameState::State::SYNC_TIME, roomSession->getElapsedTime());
	}

	// Relay the round ending to all players.
	roomSession->relayPlaying<GCPvERoundEnd>();
}

void RoomSessionPveRoundManager::tick()
{
	if (m_hasRoundEnded)
	{
		return;
	}

	switch (m_currentRound)
	{
	case eRound::CHESS_CASTLE_STAGE_1:
		checkRoundZeroFinished();
		break;
	case eRound::CHESS_CASTLE_STAGE_2:
		checkRoundOneFinished();
		break;
	case eRound::CHESS_CASTLE_STAGE_3:
		checkRoundTwoFinished();
		break;
	}
}

void RoomSessionPveRoundManager::checkRoundZeroFinished()
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	const auto players = roomSession->getPlayingPlayers();

	const int finishesNeeded = static_cast<int>(players.size());
	if (finishesNeeded == 0)
		return;

	const auto currentTime = time(nullptr);

	if (currentTime >= roomSession->getEndTime())
	{
		roomSession->finishPveGame(false);
	}

	int deadFinishes = 0;
	int actualFinishes = 0;

	constexpr auto boundsMin = Position{ 24.37f, 0.f, 25.8f };
	constexpr auto boundsMax = Position{ 40.92f, 50.f, 38.54f };

	for (const auto& player : players)
	{
		if (AABBHelper::isPositionInBetweenBounds(player->getPosition(), boundsMin, boundsMax))
		{
			actualFinishes++;
		}
		else if (player->isPermanentlyDead())
		{
			deadFinishes++;
		}
	}

	if (actualFinishes == 0)
	{
		return;
	}

	// ReSharper disable once CppTooWideScope
	const bool finishedStage = actualFinishes + deadFinishes == finishesNeeded;

	bool allRequiredAreasAreInitialized = true;

	// Now we check if all areas have been initialized/passed through.
	for (const auto& area : roomSession->getPveAreaManager()->getAllRequiredPassThroughAreas())
	{
		if (!area->isInitialized())
		{
			allRequiredAreasAreInitialized = false;
			break;
		}
	}

	if (finishedStage && allRequiredAreasAreInitialized)
	{
		endRound();
	}
}

uint32_t RoomSessionPveRoundManager::getInitializedPlayerCount() const
{
	return m_initializedPlayerCount;
}

void RoomSessionPveRoundManager::checkRoundOneFinished() const
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	const auto players = roomSession->getPlayingPlayers();

	if (players.empty())
		return;

	// ReSharper disable once CppTooWideScopeInitStatement
	const auto currentTime = time(nullptr);

	if (currentTime >= roomSession->getEndTime())
	{
		// TODO: Move to the next stage instead of finishing.
		roomSession->finishPveGame(true);

		return;
	}

	// Note: Essence object should always be uid 1.
	// ReSharper disable once CppTooWideScopeInitStatement
	const auto essenceObject = roomSession->getObjectManager()->findObjectByUid(1);

	if ((essenceObject != nullptr) && (essenceObject->getHealth() == 0))
	{
		// The essence object has no health left, this means the players have lost.
		roomSession->finishPveGame(false);

		return;
	}

	uint32_t deadPlayerCount = 0;

	for (const auto& player : players)
	{
		if (player->isPermanentlyDead())
		{
			deadPlayerCount++;
		}
	}

	if ((deadPlayerCount != 0) && (players.size() == deadPlayerCount))
	{
		// All players are dead without being able to respawn so we can finish the game.
		roomSession->finishPveGame(false);
	}
}

void RoomSessionPveRoundManager::checkRoundTwoFinished()
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	const auto players = roomSession->getPlayingPlayers();

	if (players.empty())
	{
		return;
	}

	// Note: Boss npc should always be uid 1.
	// ReSharper disable once CppTooWideScopeInitStatement
	const auto bossNpc = roomSession->getNpcManager()->findNpcByUid(1);
	// ReSharper disable once CppTooWideScope
	const auto hasBossBeenDefeated = (bossNpc != nullptr) && (bossNpc->isDead());

	if (hasBossBeenDefeated && m_endTime == NULL)
	{
		// End the game in 10 seconds after killing the boss.
		if (roomSession->getTimeLeftInSeconds() > 10)
		{
			m_endTime = (time(nullptr) + static_cast<time_t>(10));
		}
	}

	// ReSharper disable once CppTooWideScopeInitStatement
	const auto currentTime = time(nullptr);

	if ((hasBossBeenDefeated && (m_endTime != NULL) && currentTime >= m_endTime)
		|| currentTime >= roomSession->getEndTime())
	{
		roomSession->finishPveGame(hasBossBeenDefeated);

		return;
	}

	uint32_t deadPlayerCount = 0;

	for (const auto& player : players)
	{
		if (player->isPermanentlyDead())
		{
			deadPlayerCount++;
		}
	}

	if ((deadPlayerCount != 0) && (players.size() == deadPlayerCount))
	{
		// All players are dead without being able to respawn so we can finish the game.
		roomSession->finishPveGame(false);
	}
}

uint8_t RoomSessionPveRoundManager::getMap() const
{
	return m_currentMap;
}

eRound RoomSessionPveRoundManager::getCurrentRound() const
{
	return m_currentRound;
}
