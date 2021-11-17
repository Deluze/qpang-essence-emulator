#pragma once

#include <cstdint>

#include "packets/LobbyServerPacket.h"
#include "packets/writers/InventoryCardWriter.h"
#include "qpang/player/inventory/InventoryCard.h"

class SendInventoryCardExtended : public LobbyServerPacket
{
public:
	SendInventoryCardExtended(const InventoryCard& card, const uint32_t balance, const bool isCash) : LobbyServerPacket(810)
	{
		writeFlag(isCash);
		writeInt(balance);
		writeInt(0);
		writeByte(1);

		InventoryCardWriter::write(this, card);
	}
};
