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

		writeShort(size); // Total amount of elements
		writeShort(size); // How many elements have been send in total + the amount of elements in the packet.
		writeShort(size); // Total elements in the current packet.

		for (const InventoryCard& card : cards)
		{
			InventoryCardWriter::write(this, card);
		}
	}
};
