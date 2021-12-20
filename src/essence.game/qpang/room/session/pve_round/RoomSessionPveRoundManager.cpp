#include "RoomSessionPveRoundManager.h"

#include "gc_pve_new_round.hpp"
#include "gc_pve_round_end.hpp"
#include "RoomSession.h"

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

	// TODO: Perhaps check the current round.

	m_currentRound++;

	// Relay the new round to all players.
	roomSession->relayPlaying<GCPvENewRound>();
}

void RoomSessionPveRoundManager::endRound() const
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

	// Relay the round ending to all players.
	roomSession->relayPlaying<GCPvERoundEnd>();
}
