#pragma once

#include <vector>

#include "packets/LobbyServerPacket.h"
#include "packets/writers/InventoryCardWriter.h"
#include "qpang/player/inventory/InventoryCard.h"
#include "qpang/shop/ShopItem.h"

class CardPurchaseComplete : public LobbyServerPacket
{
public:
	CardPurchaseComplete(const ShopItem& item, const std::vector<InventoryCard>& cards, const uint32_t newBalance) : LobbyServerPacket(804)
	{
		writeFlag(item.isCash);
		writeInt(newBalance);
		writeInt(0); // ?
		writeByte(static_cast<uint8_t>(cards.size()));

		for (const auto& card : cards)
		{
			InventoryCardWriter::write(this, card);
		}
	}
};
