#pragma once

#include <vector>

#include "packets/LobbyServerPacket.h"
#include "packets/writers/FriendWriter.h"
#include "qpang/player/friend/Friend.h"

class AppearOnline : public LobbyServerPacket
{
public:
	AppearOnline(uint32_t playerId) : LobbyServerPacket(603)
	{
		writeInt(playerId);
	}
};