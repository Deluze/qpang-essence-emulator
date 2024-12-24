#pragma once

#include "core/communication/packet/ServerPacket.h"
#include "qpang/square/Square.h"

class SquareEntryWriter
{
public:
	static void write(ServerPacket* packet, const Square::Ptr square)
	{
		packet->writeEmpty(5);
		packet->writeInt(square->getId());
		packet->writeByte(square->getCapacity());
		packet->writeByte(square->getPlayerCount());
		packet->writeByte(square->getState());
		packet->writeString(square->getName(), 16);
		packet->writeEmpty(33);
	}
};
