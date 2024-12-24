#pragma once

#include "packets/LobbyServerPacket.h"

class SendGameRoomInvite : public LobbyServerPacket
{
public:
	explicit SendGameRoomInvite(const uint32_t host, const uint16_t port, const uint8_t mode, const std::u16string& roomTitle) : LobbyServerPacket(721)
	{
		writeInt(host);
		writeShort(port);
		
		writeShort(mode); // gamemode
		writeString(roomTitle, 30);
	}
};
