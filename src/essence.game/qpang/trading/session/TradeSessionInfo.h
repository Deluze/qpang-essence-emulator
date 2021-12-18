#pragma once

#include <qpang/player/Player.h>
#include <qpang/trading/session/participant/TradeSessionParticipant.h>

class TradeSessionInfo
{
public:
	TradeSessionInfo() {}

	TradeSessionInfo(const uint32_t buddyId, const bool isPending)
	{
		m_buddyId = buddyId;
		m_isPending = isPending;
	}

	void addCard(uint64_t cardToAdd);

	bool removeCard(uint64_t cardToRemove);

	uint32_t getBuddyId();

	void setPending(bool isPending);

	bool isPending();

	void setFinished(bool isFinished);

	bool isFinished();

	std::vector<uint64_t> getProposedCards();

private:
	uint32_t m_buddyId = 0;
	bool m_isPending = false;
	bool m_isFinished = false;
	std::vector<uint64_t> proposedCards{};
};