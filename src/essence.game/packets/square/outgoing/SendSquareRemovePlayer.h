#pragma once

#include "packets/SquareServerPacket.h"

class SendSquareRemovePlayer : public SquareServerPacket
{
public:
	explicit SendSquareRemovePlayer(const uint32_t playerId) : SquareServerPacket(6509)
	{
		writeInt(playerId);
	}
};