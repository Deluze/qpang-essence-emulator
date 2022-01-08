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
		//roomSession->getAboveGroundPathfinder()->updateMapInfo(Maps::pveStage3MapInfo);
		roomSession->getAboveGroundPathfinder()->free();
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

	printf("Increasing player count.\n");

	m_initializedPlayerCount++;

	// Check if the initialized player count equals the playing players size.
	if (m_initializedPlayerCount >= roomSession->getPlayers().size())
	{
		printf("Players initialized! Initializing everything.\n");

		// DONT! Relay the new round to all players.
		//roomSession->relayPlaying<GCPvENewRound>();

		// Reset the time.
		roomSession->resetTime();

		switch (m_currentRound)
		{
		case eRound::CHESS_CASTLE_STAGE_1:
			m_currentRound = eRound::CHESS_CASTLE_STAGE_2;
			break;
		case eRound::CHESS_CASTLE_STAGE_2:
			m_currentRound = eRound::CHESS_CASTLE_STAGE_3;
			break;
		case eRound::CHESS_CASTLE_STAGE_3:
			// TODO: Finish the game, since this is the last stage.
			return;
		}

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

		for (const auto& player : roomSession->getPlayingPlayers())
		{
			// Synchronize time for every player.
			player->send<GCGameState>(player->getPlayer()->getId(), CGGameState::State::SYNC_TIME, roomSession->getElapsedTime());
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
		// finish game or something
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

	const auto currentTime = time(nullptr);

	if (currentTime >= roomSession->getEndTime())
	{
		roomSession->finishPveGame(false);
	}

	const auto players = roomSession->getPlayingPlayers();

	const int finishesNeeded = static_cast<int>(players.size());

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

	if (finishedStage)
	{
		endRound();
	}
}

void RoomSessionPveRoundManager::checkRoundOneFinished() const
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

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

	const auto players = roomSession->getPlayingPlayers();

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
