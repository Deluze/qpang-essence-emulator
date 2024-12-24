#pragma once

#include "packets/SquareServerPacket.h"
#include "qpang/square/SquarePlayer.h"

class SendSquarePlayerPosition : public SquareServerPacket
{
public:
	explicit SendSquarePlayerPosition(const SquarePlayer::Ptr& squarePlayer) : SquareServerPacket(6531)
	{
		writeArray<float, 3>(squarePlayer->getPosition());
	}
};
