#pragma once

#include "packets/SquareServerPacket.h"

#include "qpang/player/Player.h"
#include "qpang/player/equipment/EquipmentManager.h"
#include "qpang/square/SquarePlayer.h"

#include "packets/writers/SquarePlayerWriter.h"

class AddPlayer : public SquareServerPacket
{
public:
	AddPlayer(SquarePlayer::Ptr squarePlayer) : SquareServerPacket(6507)
	{
		SquarePlayerWriter::write(this, squarePlayer);
	}
};