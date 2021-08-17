#pragma once

#include "packets/LobbyServerPacket.h"

class CancelOutgoingFriend : public LobbyServerPacket
{
public:
	explicit CancelOutgoingFriend(const uint32_t playerId) : LobbyServerPacket(710)
	{
		writeInt(playerId);
	}
};