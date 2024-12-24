#pragma once

#include <cstdint>

#include "packets/LobbyServerPacket.h"

class SendDenyIncomingFriend : public LobbyServerPacket
{
public:
	explicit SendDenyIncomingFriend(const uint32_t playerId) : LobbyServerPacket(706)
	{
		writeInt(playerId);
	}
};