#pragma once

#include "packets/LobbyServerPacket.h"

class Broadcast : public LobbyServerPacket
{
public:
	explicit Broadcast(const std::u16string& message) : LobbyServerPacket(4)
	{
		writeInt(0);
		writeEmpty(34);
		writeString(message, message.size() % 254);
	}
};