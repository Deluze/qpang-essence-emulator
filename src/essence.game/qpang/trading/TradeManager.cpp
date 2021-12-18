#include "TradeManager.h"

#include <qpang/Game.h>

#include <packets/lobby/outgoing/trading/SendTradeRequestCancel.h>
#include <packets/lobby/outgoing/trading/SendTradeCancelOther.h>
#include <packets/lobby/incoming/trading/HandleUpdateTradeStateRequest.h>

bool TradeManager::isTrading(uint32_t userId, bool ignorePending) 
{
	return m_tradeSessions.count(userId) && (ignorePending || !m_tradeSessions[userId].isPending());
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

void TradeManager::cancelActiveTrades(uint32_t userId)
{
	if (this->isTrading(userId, true))
	{
		auto tradingSessionInfo = this->getTradeSessionInfo(userId);
		auto buddyId = tradingSessionInfo.getBuddyId();

		// End trade sessions
		this->endTradeSession(userId);
		this->endTradeSession(buddyId);

		const auto targetPlayer = Game::instance()->getOnlinePlayer(buddyId);
		if (targetPlayer)
		{
			if (tradingSessionInfo.isPending())
			{
				targetPlayer->send(SendTradeRequestCancel());
			}
			else
			{
				targetPlayer->send(SendTradeCancelOther(userId, HandleUpdateTradeStateRequest::State::CANCEL_TRADE));
			}
		}
	}
}

std::vector<uint32_t> TradeManager::getUsersRequestingTrade(uint32_t targetUserId)
{
	std::vector<uint32_t> requests = {};
	for (auto tradeSession : m_tradeSessions)
	{
		if (tradeSession.second.getBuddyId() == targetUserId && tradeSession.second.isPending())
			requests.push_back(tradeSession.first);
	}

	return requests;
}

TradeSessionInfo& TradeManager::getTradeSessionInfo(uint32_t userId)
{
	return m_tradeSessions[userId];
}