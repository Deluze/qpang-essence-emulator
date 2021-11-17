#pragma once

#include <cstdint>

#include "packets/SquareServerPacket.h"
#include "qpang/square/Square.h"
#include "qpang/square/SquarePlayer.h"

class SendJoinSquareSuccess : public SquareServerPacket
{
public:
	explicit SendJoinSquareSuccess(const SquarePlayer::Ptr& squarePlayer) : SquareServerPacket(6538)
	{
		const auto square = squarePlayer->getSquare();

		writeShort(static_cast<uint16_t>(square->getId())); // dont even ask, header text 'square - ID'
		writeEmpty(7);
		writeInt(square->getId());
		writeByte(square->getCapacity());
		writeByte(square->getPlayerCount());
		writeByte(square->getState());
		writeString(square->getName(), 16);
		writeEmpty(33);
		writeArray<float, 3>({0, 0, 0});

	}
};
