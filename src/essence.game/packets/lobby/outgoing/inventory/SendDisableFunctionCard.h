#pragma once

#include "packets/LobbyServerPacket.h"
#include "packets/writers/InventoryCardWriter.h"
#include "qpang/player/inventory/InventoryCard.h"

class SendDisableFunctionCard : public LobbyServerPacket
{
public:
	explicit SendDisableFunctionCard(const InventoryCard& card) : LobbyServerPacket(862)
	{
		InventoryCardWriter::write(this, card);
	}
};
