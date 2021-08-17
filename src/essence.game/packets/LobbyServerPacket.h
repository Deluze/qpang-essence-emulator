#pragma once

#include "core/communication/packet/ServerPacket.h"

class LobbyServerPacket : public ServerPacket
{
public:
	explicit LobbyServerPacket(const uint16_t packetId) : ServerPacket(packetId)
	{
	}
};