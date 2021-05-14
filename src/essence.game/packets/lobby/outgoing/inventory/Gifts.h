#pragma once

#include <vector>

#include "packets/LobbyServerPacket.h"
#include "packets/writers/GiftWriter.h"

#include "qpang/player/inventory/InventoryCard.h"

class Gifts : public LobbyServerPacket
{
public:
	Gifts(const std::vector<InventoryCard>& gifts) : LobbyServerPacket(746)
	{
		const uint16_t size = static_cast<uint16_t>(gifts.size());

		writeShort(size);
		writeShort(size);
		writeShort(size);

		for (const auto& gift : gifts)
			GiftWriter::write(this, gift);
	}
};