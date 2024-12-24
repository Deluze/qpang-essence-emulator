#include "TradeSessionInfo.h"

void TradeSessionInfo::addCard(uint64_t cardToAdd)
{
    proposedCards.push_back(cardToAdd);
}

bool TradeSessionInfo::removeCard(uint64_t cardToRemove)
{
    auto pos = std::find(proposedCards.begin(), proposedCards.end(), cardToRemove);
    if (pos != proposedCards.end())
    {
        proposedCards.erase(pos);
        return true;
    }

    // Card wasn't added in the first place?
    return false;
}

uint32_t TradeSessionInfo::getBuddyId()
{
    return m_buddyId;
}

bool TradeSessionInfo::isPending()
{
    return m_isPending;
}

void TradeSessionInfo::setFinished(bool isFinished)
{
    m_isFinished = isFinished;
}

bool TradeSessionInfo::isFinished()
{
    return m_isFinished;
}

void TradeSessionInfo::setPending(bool isPending)
{
	m_isPending = isPending;
}

std::vector<uint64_t> TradeSessionInfo::getProposedCards()
{
    return proposedCards;
}