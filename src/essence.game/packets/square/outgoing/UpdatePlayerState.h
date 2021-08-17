#pragma once

#include "packets/SquareServerPacket.h"
#include "qpang/player/Player.h"
#include "qpang/square/SquarePlayer.h"

class UpdatePlayerState : public SquareServerPacket
{
public:
	UpdatePlayerState(const SquarePlayer::Ptr squarePlayer, const uint8_t value) : SquareServerPacket(6547)
	{
		writeInt(squarePlayer->getPlayer()->getId());
		writeInt(squarePlayer->getState());
		writeByte(value);
		writeByte(0); // ?
	}
};
