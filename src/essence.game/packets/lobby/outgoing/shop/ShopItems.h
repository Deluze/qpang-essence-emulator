#pragma once

#include <vector>

#include "packets/LobbyServerPacket.h"
#include "qpang/shop/ShopItem.h"

class ShopItems : public LobbyServerPacket
{
public:
	explicit ShopItems(const std::vector<ShopItem>& items) : LobbyServerPacket(798)
	{
		const auto size = static_cast<uint16_t>(items.size());

		writeShort(size);
		writeShort(size);
		writeShort(size);

		// ReSharper disable once CppUseStructuredBinding
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
