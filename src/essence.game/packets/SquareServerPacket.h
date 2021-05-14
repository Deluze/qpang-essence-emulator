#pragma once

#include "core/communication/packet/ServerPacket.h"

class SquareServerPacket : public ServerPacket
{
public:
	SquareServerPacket(uint16_t packetId) : ServerPacket(packetId)
	{
	}
};