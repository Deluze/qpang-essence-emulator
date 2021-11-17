#pragma once

#include "packets/LobbyServerPacket.h"

class SendAppearOffline : public LobbyServerPacket
{
public:
	explicit SendAppearOffline(const uint32_t playerId) : LobbyServerPacket(607)
	{
		writeInt(playerId);
	}
};