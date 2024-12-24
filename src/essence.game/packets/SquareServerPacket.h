#pragma once

#include "core/communication/packet/ServerPacket.h"

class SquareServerPacket : public ServerPacket
{
public:
	explicit SquareServerPacket(const uint16_t packetId) : ServerPacket(packetId)
	{
	}
};