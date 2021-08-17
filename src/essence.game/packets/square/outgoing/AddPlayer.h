#pragma once

#include "packets/SquareServerPacket.h"
#include "packets/writers/SquarePlayerWriter.h"
#include "qpang/square/SquarePlayer.h"

class AddPlayer : public SquareServerPacket
{
public:
	explicit AddPlayer(SquarePlayer::Ptr squarePlayer) : SquareServerPacket(6507)
	{
		SquarePlayerWriter::write(this, squarePlayer);
	}
};
