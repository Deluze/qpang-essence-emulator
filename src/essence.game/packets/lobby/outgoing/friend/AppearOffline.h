#pragma once

#include <vector>

#include "packets/LobbyServerPacket.h"
#include "packets/writers/FriendWriter.h"
#include "qpang/player/friend/Friend.h"

class AppearOffline : public LobbyServerPacket
{
public:
	AppearOffline(uint32_t playerId) : LobbyServerPacket(607)
	{
		writeInt(playerId);
	}
};