#pragma once

#include "packets/LobbyServerPacket.h"
#include "packets/writers/InventoryCardWriter.h"
#include "qpang/player/inventory/InventoryCard.h"

class EnableFunctionCard : public LobbyServerPacket
{
public:
	explicit EnableFunctionCard(const InventoryCard& card) : LobbyServerPacket(835)
	{
		InventoryCardWriter::write(this, card);
	}
};
