#include "RoomSessionPveRoundManager.h"

#include "gc_pve_new_round.hpp"
#include "gc_pve_round_end.hpp"
#include "RoomSession.h"

#include <qpang/helpers/AABBHelper.h>

void RoomSessionPveRoundManager::initialize(const std::shared_ptr<RoomSession>& roomSession)
{
	m_roomSession = roomSession;
}

void RoomSessionPveRoundManager::onStartNewRound()
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	m_currentRound++;

	roomSession->resetTime();

	// Relay the new round to all players.
	roomSession->relayPlaying<GCPvENewRound>();
}

void RoomSessionPveRoundManager::endRound()
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	// Can not end this round since it's the last round.
	if (m_currentRound == 2)
	{
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
		return;

	switch (m_currentRound)
	{
	case 0:
		checkRoundZeroFinished();
		break;
	}
}

void RoomSessionPveRoundManager::checkRoundZeroFinished()
{
	const auto roomSession = m_roomSession.lock();
	if (roomSession == nullptr)
		return;

	const auto players = roomSession->getPlayingPlayers();

	const int finishesNeeded = players.size();
	int deadFinishes = 0;
	int actualFinishes = 0;

	const auto boundsMin = Position{ 24.37f, 0.f, 25.8f };
	const auto boundsMax = Position{ 40.92f, 50.f, 38.54f };

	for (const auto& player : players)
	{
		if (AABBHelper::isPositionInBetweenBounds(player->getPosition(), boundsMin, boundsMax))
			actualFinishes++;
		else if (player->isPermanentlyDead())
			deadFinishes++;
	}

	if (actualFinishes == 0)
		return;

	bool finishedStage = actualFinishes + deadFinishes == finishesNeeded;
	if (finishedStage)
		endRound();
}