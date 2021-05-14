#pragma once

#include <cstdint>

#include "packets/LobbyServerPacket.h"

class DenyIncomingFriend : public LobbyServerPacket
{
public:
	DenyIncomingFriend(const uint32_t playerId) : LobbyServerPacket(706)
	{
		writeInt(playerId);
	}
};