#pragma once

#include "packets/LobbyServerPacket.h"

class SendMessageBroadcast : public LobbyServerPacket
{
public:
	explicit SendMessageBroadcast(const std::u16string& message) : LobbyServerPacket(4)
	{
		writeInt(0);
		writeEmpty(34);
		writeString(message, message.size() % 254);
	}
};