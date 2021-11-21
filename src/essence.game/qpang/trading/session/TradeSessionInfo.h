#pragma once

#include <qpang/player/Player.h>
#include <qpang/trading/session/participant/TradeSessionParticipant.h>

class TradeSessionInfo
{
public:
	TradeSessionInfo(const uint32_t buddyId, const bool isPending)
	{
		m_buddyId = buddyId;
		m_isPending = isPending;
	}

	uint32_t getBuddyId();
	bool isPending();

private:
	uint32_t m_buddyId;
	bool m_isPending;
};