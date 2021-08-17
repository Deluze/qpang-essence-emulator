#pragma once

#include <cstdint>

#include "packets/SquareServerPacket.h"

class UpdatePlayerLevel : public SquareServerPacket
{
public:
	UpdatePlayerLevel(const uint32_t playerId, const uint8_t level) : SquareServerPacket(6553)
	{
		writeInt(playerId);
		writeByte(level);
	}
};