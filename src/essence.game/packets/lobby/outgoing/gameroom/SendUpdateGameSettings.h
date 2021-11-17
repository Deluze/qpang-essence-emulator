#pragma once

#include <cstdint>

#include "packets/LobbyServerPacket.h"

class SendUpdateGameSettings : public LobbyServerPacket
{
public:
	SendUpdateGameSettings(const uint32_t host, const uint16_t port, const bool isEnabled) : LobbyServerPacket(770)
	{
		writeFlag(isEnabled);
		writeInt(host);
		writeShort(port);
		writeEmpty(42);
	}
};