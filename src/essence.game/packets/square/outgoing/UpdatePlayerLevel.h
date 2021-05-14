#pragma once

#include <cstdint>

#include "packets/SquareServerPacket.h"

class UpdatePlayerLevel : public SquareServerPacket
{
public:
	UpdatePlayerLevel(uint32_t playerId, uint8_t level) : SquareServerPacket(6553)
	{
		writeInt(playerId);
		writeByte(level);
	}
};