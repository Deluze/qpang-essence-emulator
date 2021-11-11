#pragma once

#include <vector>

#include "packets/LobbyServerPacket.h"
#include "packets/writers/InventoryCardWriter.h"
#include "qpang/player/inventory/InventoryCard.h"

class Inventory : public LobbyServerPacket
{
public:
	explicit Inventory(const std::vector<InventoryCard>& cards, const uint16_t totalItemCount,
	                   const uint16_t currentlySendCount, const uint16_t currentItemCount) : LobbyServerPacket(781)
	{
		writeShort(totalItemCount);
		writeShort(currentlySendCount);
		writeShort(currentItemCount);

		for (const InventoryCard& card : cards)
		{
			InventoryCardWriter::write(this, card);
		}
	}
};
