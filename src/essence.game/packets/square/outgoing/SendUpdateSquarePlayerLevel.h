#pragma once

#include <cstdint>

#include "packets/SquareServerPacket.h"

class SendUpdateSquarePlayerLevel : public SquareServerPacket
{
public:
	SendUpdateSquarePlayerLevel(const uint32_t playerId, const uint8_t level) : SquareServerPacket(6553)
	{
		writeInt(playerId);
		writeByte(level);
	}
};