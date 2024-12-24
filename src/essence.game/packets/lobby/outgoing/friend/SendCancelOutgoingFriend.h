#pragma once

#include "packets/LobbyServerPacket.h"

class SendCancelOutgoingFriend : public LobbyServerPacket
{
public:
	explicit SendCancelOutgoingFriend(const uint32_t playerId) : LobbyServerPacket(710)
	{
		writeInt(playerId);
	}
};