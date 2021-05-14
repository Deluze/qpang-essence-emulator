#pragma once

#include <string>

#include "packets/LobbyServerPacket.h"

class SendWhisper : public LobbyServerPacket
{
public:
	SendWhisper(const std::u16string& sender, const std::u16string& message) : LobbyServerPacket(739)
	{
		writeInt(0);
		writeShort(message.size());
		writeString(sender, 16);
		writeString(message, message.size() % 254);
	}
};