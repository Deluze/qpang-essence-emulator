#pragma once

#include "packets/LobbyServerPacket.h"
#include "packets/writers/InventoryCardWriter.h"
#include "qpang/player/inventory/InventoryCard.h"

class SendEnableFunctionCard : public LobbyServerPacket
{
public:
	explicit SendEnableFunctionCard(const InventoryCard& card) : LobbyServerPacket(835)
	{
		InventoryCardWriter::write(this, card);
	}
};
