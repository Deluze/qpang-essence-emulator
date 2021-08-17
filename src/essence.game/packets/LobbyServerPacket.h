#pragma once

#include "core/communication/packet/ServerPacket.h"

class LobbyServerPacket : public ServerPacket
{
public:
	LobbyServerPacket(uint16_t packetId) : ServerPacket(packetId)
	{
	}
};