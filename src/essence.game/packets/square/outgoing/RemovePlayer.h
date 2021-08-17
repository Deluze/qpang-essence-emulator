#pragma once

#include "packets/SquareServerPacket.h"

class RemovePlayer : public SquareServerPacket
{
public:
	explicit RemovePlayer(const uint32_t playerId) : SquareServerPacket(6509)
	{
		writeInt(playerId);
	}
};