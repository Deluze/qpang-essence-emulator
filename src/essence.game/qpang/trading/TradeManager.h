#pragma once

#include <vector>

#include <qpang/trading/session/TradeSessionInfo.h>

class TradeManager
{
public:
#pragma region TradeSessions
	/// <summary>
	/// Determines whether or not a player is in a trade.
	/// </summary>
	bool isTrading(const uint32_t playerId);

	/// <summary>
	/// Determines whether or not a trade session is pending.
	/// </summary>
	bool isPendingTradeSession(const uint32_t playerId);

	/// <summary>
	/// Starts the trading session for two players.
	/// </summary>
	void startTradeSession(const uint32_t playerId, const uint32_t buddyId, const bool isPending);

	/// <summary>
	/// Ends the trading session for a player.
	/// </summary>
	void endTradeSession(const uint32_t playerId);
#pragma endregion

#pragma region TradingBuddy
	/// <summary>
	/// Finds the corresponding trading buddy in the trading session.
	/// </summary>
	uint32_t findTradingBuddyId(const uint32_t playerId);
#pragma endregion

private:
	std::map<uint32_t, TradeSessionInfo*> m_tradeSessions;
};