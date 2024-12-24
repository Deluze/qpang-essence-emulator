#pragma once

#include "packets/SquareServerPacket.h"
#include "packets/writers/SquarePlayerWriter.h"
#include "qpang/square/SquarePlayer.h"

class SendAddSquarePlayer : public SquareServerPacket
{
public:
	explicit SendAddSquarePlayer(const SquarePlayer::Ptr& squarePlayer) : SquareServerPacket(6507)
	{
		SquarePlayerWriter::write(this, squarePlayer);
	}
};
