#pragma once

#include <string>

#include "packets/LobbyServerPacket.h"

class ReceiveWhisper : public LobbyServerPacket
{
public:
	ReceiveWhisper(const std::u16string& sender, const std::u16string& message) : LobbyServerPacket(738)
	{
		writeInt(123123);
		writeShort(message.size());
		writeString(sender, 16);
		writeString(message, message.size() % 254);
	}
};