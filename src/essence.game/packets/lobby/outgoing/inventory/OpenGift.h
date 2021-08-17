#pragma once

#include "packets/LobbyServerPacket.h"
#include "packets/writers/InventoryCardWriter.h"
#include "qpang/player/inventory/InventoryCard.h"

class OpenGiftSuccess : public LobbyServerPacket
{
public:
	explicit OpenGiftSuccess(const InventoryCard& card) : LobbyServerPacket(743)
	{
		writeInt(0);
		writeInt(0);

		writeInt(1); // card count

		InventoryCardWriter::write(this, card);
	}
};
