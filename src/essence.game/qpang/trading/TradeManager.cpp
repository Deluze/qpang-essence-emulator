#include "TradeManager.h"

bool TradeManager::isTrading(uint32_t userId) 
{
	return m_tradeSessions.count(userId) && !m_tradeSessions[userId].isPending();
}

void TradeManager::startTradeSession(uint32_t userId, uint32_t targetUserId, bool isPending)
{
	m_tradeSessions[userId] = TradeSessionInfo(targetUserId, isPending);
}

void TradeManager::endTradeSession(uint32_t userId)
{
	if (m_tradeSessions.count(userId))
	{
		m_tradeSessions.erase(userId);
	}
}

bool TradeManager::acceptTradeSession(uint32_t userId, uint32_t targetUserId)
{
	if (!m_tradeSessions.count(targetUserId))
	{
		return false;
	}

	// Update requester trade session to not be pending anymore
	m_tradeSessions[targetUserId].setPending(false);

	// Add trade session to requestor for the current userid
	m_tradeSessions[userId] = TradeSessionInfo(targetUserId, false);
}

TradeSessionInfo& TradeManager::getTradeSessionInfo(uint32_t userId)
{
	return m_tradeSessions[userId];
}