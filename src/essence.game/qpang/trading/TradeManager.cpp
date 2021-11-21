#include "TradeManager.h"

bool TradeManager::isTrading(const uint32_t playerId)
{
	const auto it = m_ongoingTradeSessions.find(playerId);

	return (it != m_ongoingTradeSessions.end() && !it->second->isPending());
}

void TradeManager::startTradeSession(const uint32_t playerId, const uint32_t buddyId, const bool isPending)
{
	m_ongoingTradeSessions[playerId] = new TradeSessionInfo(buddyId, isPending);
}

void TradeManager::endTradeSession(const uint32_t playerId)
{
	m_ongoingTradeSessions.erase(playerId);
}

uint32_t TradeManager::findTradingBuddyId(const uint32_t playerId)
{
	const auto it = m_ongoingTradeSessions.find(playerId);

	if (it != m_ongoingTradeSessions.end())
	{
		return it->second->getBuddyId();
	}

	return 0;
}