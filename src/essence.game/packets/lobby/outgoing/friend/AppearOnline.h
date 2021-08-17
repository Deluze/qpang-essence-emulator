#pragma once

#include "packets/LobbyServerPacket.h"
#include "packets/writers/FriendWriter.h"

class AppearOnline : public LobbyServerPacket
{
public:
	explicit AppearOnline(const uint32_t playerId) : LobbyServerPacket(603)
	{
		writeInt(playerId);
	}
};