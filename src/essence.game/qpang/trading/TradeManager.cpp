#include "TradeManager.h"

bool TradeManager::isTrading(const uint32_t playerId)
{
	const auto it = m_tradeSessions.find(playerId);

	return (it != m_tradeSessions.end() && !it->second->isPending());
}

bool TradeManager::isPendingTradeSession(const uint32_t playerId)
{
	const auto it = m_tradeSessions.find(playerId);

	if (it != m_tradeSessions.end())
	{
		return (it->second->isPending());
	}

	return false;
}

void TradeManager::startTradeSession(const uint32_t playerId, const uint32_t buddyId, const bool isPending)
{
	m_tradeSessions[playerId] = new TradeSessionInfo(buddyId, isPending);
}

void TradeManager::endTradeSession(const uint32_t playerId)
{
	m_tradeSessions.erase(playerId);
}

TradeSessionInfo TradeManager::getTradeSessionInfo(const uint32_t playerId)
{
	return *m_tradeSessions[playerId];
}

uint32_t TradeManager::findTradingBuddyId(const uint32_t playerId)
{
	const auto it = m_tradeSessions.find(playerId);

	if (it != m_tradeSessions.end())
	{
		return it->second->getBuddyId();
	}

	return 0;
}