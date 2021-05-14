#pragma once

#include "packets/LobbyServerPacket.h"

#include "qpang/player/inventory/InventoryCard.h"
#include "packets/writers/InventoryCardWriter.h"

class OpenGiftSuccess : public LobbyServerPacket
{
public:
	OpenGiftSuccess(const InventoryCard& card) : LobbyServerPacket(743)
	{
		writeInt(0);
		writeInt(0);

		writeInt(1); // card count

		InventoryCardWriter::write(this, card);
	}
};