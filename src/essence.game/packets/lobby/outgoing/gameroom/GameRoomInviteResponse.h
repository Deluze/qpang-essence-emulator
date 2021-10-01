#pragma once

#include <vector>

#include "packets/LobbyServerPacket.h"

class GameRoomInviteResponse : public LobbyServerPacket
{
public:
	explicit GameRoomInviteResponse(uint32_t host, uint16_t port, uint8_t mode, std::u16string roomTitle) : LobbyServerPacket(721)
	{
		writeInt(host);
		writeShort(port);
		
		writeShort(mode); // gamemode
		writeString(roomTitle.c_str(), 30);
	}
};
