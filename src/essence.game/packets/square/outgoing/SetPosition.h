#pragma once

#include "packets/SquareServerPacket.h"
#include "qpang/square/SquarePlayer.h"

class SetPosition : public SquareServerPacket
{
public:
	explicit SetPosition(const SquarePlayer::Ptr squarePlayer) : SquareServerPacket(6531)
	{
		writeArray<float, 3>(squarePlayer->getPosition());
	}
};
