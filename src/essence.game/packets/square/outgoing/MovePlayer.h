#pragma once

#include "packets/SquareServerPacket.h"

class MovePlayer : public SquareServerPacket
{
public:
	MovePlayer(const uint32_t playerId, const std::array<float, 3>& newPosition, const uint8_t moveType, const uint8_t direction) : SquareServerPacket(6513)
	{
		writeInt(playerId);
		writeByte(moveType);
		writeByte(direction);
		writeArray<float, 3>(newPosition);
	}
};