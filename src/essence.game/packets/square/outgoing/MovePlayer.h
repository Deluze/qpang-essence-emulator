#pragma once

#include "packets/SquareServerPacket.h"

class MovePlayer : public SquareServerPacket
{
public:
	MovePlayer(uint32_t playerId, std::array<float, 3>& newPosition, uint8_t moveType, uint8_t direction) : SquareServerPacket(6513)
	{
		writeInt(playerId);
		writeByte(moveType);
		writeByte(direction);
		writeArray<float, 3>(newPosition);
	}
};