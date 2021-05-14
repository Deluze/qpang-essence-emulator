#pragma once

#include <cstdint>

#include "packets/LobbyServerPacket.h"
#include "packets/writers/InventoryCardWriter.h"

#include "qpang/player/inventory/InventoryCard.h"

class CardExtended : public LobbyServerPacket
{
public:
	CardExtended(const InventoryCard& card, uint32_t balance, bool isCash) : LobbyServerPacket(810)
	{
		writeFlag(isCash);
		writeInt(balance);
		writeInt(0);
		writeByte(1);

		InventoryCardWriter::write(this, card);
	}
};