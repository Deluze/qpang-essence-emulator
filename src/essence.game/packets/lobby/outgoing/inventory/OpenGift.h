#pragma once

#include "packets/LobbyServerPacket.h"
#include "packets/writers/InventoryCardWriter.h"
#include "qpang/player/inventory/InventoryCard.h"

class SendOpenGiftSuccess : public LobbyServerPacket
{
public:
	explicit SendOpenGiftSuccess(const InventoryCard& card) : LobbyServerPacket(743)
	{
		writeInt(0);
		writeInt(0);

		writeInt(1); // card count

		InventoryCardWriter::write(this, card);
	}
};
