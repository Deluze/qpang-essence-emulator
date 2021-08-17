#pragma once

#include <vector>

#include "packets/LobbyServerPacket.h"
#include "packets/writers/InventoryCardWriter.h"
#include "qpang/player/inventory/InventoryCard.h"

class Inventory : public LobbyServerPacket
{
public:
	explicit Inventory(const std::vector<InventoryCard>& cards) : LobbyServerPacket(781)
	{
		const uint16_t size = static_cast<uint16_t>(cards.size());

		writeShort(size);
		writeShort(size);
		writeShort(size);

		for (const InventoryCard& card : cards)
		{
			InventoryCardWriter::write(this, card);
		}
	}
};
