#pragma once

#include <vector>

#include "packets/LobbyServerPacket.h"
#include "qpang/shop/ShopItem.h"

class SendShopItems : public LobbyServerPacket
{
public:
	explicit SendShopItems(const std::vector<ShopItem>& items, const uint16_t totalItemCount, const uint16_t currentlySendCount,
	                   const uint16_t currentItemCount) : LobbyServerPacket(798)
	{
		writeShort(totalItemCount);
		writeShort(currentlySendCount);
		writeShort(currentItemCount);

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
