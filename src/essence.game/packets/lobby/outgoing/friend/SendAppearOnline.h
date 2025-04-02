#pragma once

#include "packets/LobbyServerPacket.h"
#include "packets/writers/FriendWriter.h"

class SendAppearOnline : public LobbyServerPacket
{
public:
	explicit SendAppearOnline(const uint32_t playerId) : LobbyServerPacket(603)
	{
		writeInt(playerId);
	}
};