#pragma once

#include <cstdint>

#include "packets/LobbyServerPacket.h"

class SendRemoveFriend : public LobbyServerPacket
{
public:
	explicit SendRemoveFriend(const uint32_t playerId) : LobbyServerPacket(714)
	{
		writeInt(playerId);
	}
};