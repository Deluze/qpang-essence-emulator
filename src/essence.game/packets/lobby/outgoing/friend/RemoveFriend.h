#pragma once

#include <cstdint>

#include "packets/LobbyServerPacket.h"

class RemoveFriend : public LobbyServerPacket
{
public:
	explicit RemoveFriend(const uint32_t playerId) : LobbyServerPacket(714)
	{
		writeInt(playerId);
	}
};