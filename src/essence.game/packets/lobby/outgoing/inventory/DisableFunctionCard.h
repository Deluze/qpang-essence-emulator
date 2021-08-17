#pragma once

#include "packets/LobbyServerPacket.h"
#include "packets/writers/InventoryCardWriter.h"
#include "qpang/player/inventory/InventoryCard.h"

class DisableFunctionCard : public LobbyServerPacket
{
public:
	explicit DisableFunctionCard(const InventoryCard& card) : LobbyServerPacket(862)
	{
		InventoryCardWriter::write(this, card);
	}
};
