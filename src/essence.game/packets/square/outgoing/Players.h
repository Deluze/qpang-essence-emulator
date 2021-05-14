#pragma once

#include <vector>

#include "packets/SquareServerPacket.h"
#include "qpang/square/SquarePlayer.h"
#include "qpang/player/Player.h"

#include "packets/writers/SquarePlayerWriter.h"

class Players : public SquareServerPacket
{
public:
	Players(const std::vector<SquarePlayer::Ptr>& players, const uint32_t ourId) : SquareServerPacket(6508)
	{
		//10010u
		
		const auto size = players.size();
		
		writeShort(size);
		writeShort(size);
		writeShort(size);

		for (size_t i = 0; i < 100; i++) // 100 * 100 = 10000 + short sizes => 6 => 10006 => + 4 at the end?
		{
			if (i < players.size())
				SquarePlayerWriter::write(this, players.at(i));
			else
				writeEmpty(100, 0);
		}
		
		writeInt(0);
	}
};