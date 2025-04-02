#pragma once

#include <iostream>
#include <vector>

#include "packets/LobbyServerPacket.h"
#include "packets/writers/GiftWriter.h"
#include "qpang/player/inventory/InventoryCard.h"

class SendGifts : public LobbyServerPacket
{
public:
	explicit SendGifts(const std::vector<InventoryCard>& gifts, const uint16_t totalItemCount,
	               const uint16_t currentlySendCount, const uint16_t currentItemCount) : LobbyServerPacket(746)
	{
		writeShort(totalItemCount);
		writeShort(currentlySendCount);
		writeShort(currentItemCount);

		for (const auto& gift : gifts)
		{
			GiftWriter::write(this, gift);
		}
	}
};
