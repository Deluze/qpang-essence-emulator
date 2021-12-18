#include "TradeSessionInfo.h"

uint32_t TradeSessionInfo::getBuddyId()
{
    return m_buddyId;
}

bool TradeSessionInfo::isPending()
{
    return m_isPending;
}

bool TradeSessionInfo::hasLockedIn()
{
    return m_hasLockedIn;
}
