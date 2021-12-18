#pragma once

#include <vector>

#include <qpang/trading/session/TradeSessionInfo.h>

class TradeManager
{
public:
	bool isTrading(uint32_t userId);
	void startTradeSession(uint32_t userId, uint32_t targetUserId, bool isPending);
	void endTradeSession(uint32_t userId);
	bool acceptTradeSession(uint32_t userId, uint32_t targetUserId);

	std::vector<uint32_t> getUsersRequestingTrade(uint32_t targetUserId);
	TradeSessionInfo& getTradeSessionInfo(uint32_t userId);
private:
	std::map<uint32_t, TradeSessionInfo> m_tradeSessions;
};