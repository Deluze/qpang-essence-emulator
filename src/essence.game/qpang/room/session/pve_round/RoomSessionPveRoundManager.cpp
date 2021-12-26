#include "RoomSessionPveRoundManager.h"

#include "gc_pve_new_round.hpp"
#include "gc_pve_round_end.hpp"
#include "RoomSession.h"

#include <qpang/helpers/AABBHelper.h>

#include "Maps.h"

void RoomSessionPveRoundManager::initialize(const std::shared_ptr<RoomSession>& roomSession)
{
	m_roomSession = roomSession;
}

void RoomSessionPveRoundManager::onStart()
{
	updatePathfinders();
}

void RoomSessionPveRoundManager::updatePathfinders()
{
	const auto roomSession = m_roomSession.lock();
	if (roomSession == nullptr)
		return;

	switch (m_currentRound)
	{
	case 0:
		roomSession->getAboveGroundPathfinder()->updateMapInfo(Maps::pveStage1AboveGroundMapInfo);
		roomSession->getUnderGroundPathfinder()->updateMapInfo(Maps::pveStage1UnderGroundMapInfo);
		break;
	case 1:
		//roomSession->getAboveGroundPathfinder()->updateMapInfo(Maps::pveStage2MapInfo);
		roomSession->getUnderGroundPathfinder()->free();
		break;
	case 2:
		//roomSession->getAboveGroundPathfinder()->updateMapInfo(Maps::pveStage3MapInfo);
		roomSession->getUnderGroundPathfinder()->free();
		break;
	}
}

void RoomSessionPveRoundManager::onStartNewRound()
{
	const auto roomSession = m_roomSession.lock();
	if (roomSession == nullptr)
		return;

	m_currentRound++;

	updatePathfinders();

	roomSession->resetTime();

	roomSession->getRoom()->setMap(roomSession->getRoom()->getMap() + 1);

	roomSession->getObjectManager()->initializeObjects();
	roomSession->getNpcManager()->initializeNpcs();

	// Update health back to full for all playing players.
	for (const auto& player : roomSession->getPlayingPlayers())
	{
		player->setHealth(player->getDefaultHealth(), true);
	}

	// Relay the new round to all players.
	roomSession->relayPlaying<GCPvENewRound>();
}

void RoomSessionPveRoundManager::endRound()
{
	const auto roomSession = m_roomSession.lock();
	if (roomSession == nullptr)
		return;

	// Can not end this round since it's the last round.
	if (m_currentRound == 2)
	{
		// TODO: Finish the game and send result screen?
		return;
	}

	m_roundEnded = true;

	roomSession->getObjectManager()->removeAll();
	roomSession->getNpcManager()->removeAll();
	roomSession->getPveItemManager()->removeAll();

	// Relay the round ending to all players.
	roomSession->relayPlaying<GCPvERoundEnd>();
}

void RoomSessionPveRoundManager::tick()
{
	if (m_roundEnded)
	{
		return;
	}

	switch (m_currentRound)
	{
	case 0:
		checkRoundZeroFinished();
		break;
	default:
		break;
	}
}

void RoomSessionPveRoundManager::checkRoundZeroFinished()
{
	const auto roomSession = m_roomSession.lock();
	if (roomSession == nullptr)
		return;

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