#pragma once

#include <vector>

#include "packets/LobbyServerPacket.h"
#include "qpang/shop/ShopItem.h"

class ShopItems : public LobbyServerPacket
{
public:
	ShopItems(const std::vector<ShopItem>& items) : LobbyServerPacket(798)
	{
		const uint16_t size = static_cast<uint16_t>(items.size());

		writeShort(size);
		writeShort(size);
		writeShort(size);

		for (const ShopItem& item : items)
		{
			writeInt(item.seqId);
			writeInt(item.itemId);
			writeFlag(item.isCash);
			writeInt(item.price);
			writeInt(item.stock == 9999 || item.soldCount < item.stock ? 1 : 0);
			writeByte(item.shopCategory);
		}
	}
};
