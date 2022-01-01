#include "RoomSessionPveRoundManager.h"

#include "gc_pve_new_round.hpp"
#include "gc_pve_round_end.hpp"
#include "RoomSession.h"

#include <qpang/helpers/AABBHelper.h>

#include "Maps.h"

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

	switch (static_cast<eRound>(m_currentRound))
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
	default:
		break;
	}
}

void RoomSessionPveRoundManager::onStartNewRound()
{
	const auto roomSession = m_roomSession.lock();
	if (roomSession == nullptr)
		return;

	m_currentRound++;
	m_currentMap++;

	updatePathfinders();

	roomSession->resetTime();

	// Relay the new round to all players.
	roomSession->relayPlaying<GCPvENewRound>();

	// Respawn all players
	for (const auto& player : roomSession->getPlayingPlayers())
		player->respawn();

	roomSession->getPveAreaManager()->initializeAreas();
	roomSession->getObjectManager()->initializeObjects();
	roomSession->getNpcManager()->initializeNpcs();
	roomSession->getPveItemManager()->initializeItems();

	if (static_cast<eRound>(m_currentRound) == eRound::CHESS_CASTLE_STAGE_2)
	{
		roomSession->getPveWaveManager()->initializeWaves();
	}
}

void RoomSessionPveRoundManager::endRound()
{
	const auto roomSession = m_roomSession.lock();
	if (roomSession == nullptr)
		return;

	// Can not end this round since it's the last round.
	if (static_cast<eRound>(m_currentRound) == eRound::CHESS_CASTLE_STAGE_3)
	{
		// TODO: Finish the game and send result screen?
		return;
	}

	m_hasRoundEnded = true;

	roomSession->getPveAreaManager()->removeAll();
	roomSession->getObjectManager()->removeAll();
	roomSession->getNpcManager()->removeAll();
	roomSession->getPveItemManager()->removeAll();
	roomSession->getPveWaveManager()->removeAll();

	// Relay the round ending to all players.
	roomSession->relayPlaying<GCPvERoundEnd>();
}

void RoomSessionPveRoundManager::tick() const
{
	if (m_hasRoundEnded)
	{
		return;
	}

	switch (static_cast<eRound>(m_currentRound))
	{
	case eRound::CHESS_CASTLE_STAGE_1:
		checkRoundZeroFinished();
		break;
	case eRound::CHESS_CASTLE_STAGE_2:
	case eRound::CHESS_CASTLE_STAGE_3:
		break;
	}
}

void RoomSessionPveRoundManager::checkRoundZeroFinished() const
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
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
		//endRound();
		roomSession->finish();
	}
}

uint8_t RoomSessionPveRoundManager::getMap() const
{
	return m_currentMap;
}

uint8_t RoomSessionPveRoundManager::getCurrentRound() const
{
	return m_currentRound;
}
