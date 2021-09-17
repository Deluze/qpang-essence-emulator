#pragma once

#include <cstdint>

#include "packets/SquareServerPacket.h"
#include "qpang/square/Square.h"
#include "qpang/square/SquarePlayer.h"

class JoinSquareSuccess : public SquareServerPacket
{
public:
	explicit JoinSquareSuccess(const SquarePlayer::Ptr player) : SquareServerPacket(6538)
	{
		const auto square = player->getSquare();
		//const auto [x, y, z] = Game::instance()->getSpawnManager()->getRandomPlazaSpawnLocation();

		writeShort(static_cast<uint16_t>(square->getId())); // dont even ask, header text 'square - ID'
		writeEmpty(7);
		writeInt(square->getId());
		writeByte(square->getCapacity());
		writeByte(square->getPlayerCount());
		writeByte(square->getState());
		writeString(square->getName(), 16);
		writeEmpty(33);
		writeArray<float, 3>({0, 0, 0}); // player->getPosition()

	}
};
