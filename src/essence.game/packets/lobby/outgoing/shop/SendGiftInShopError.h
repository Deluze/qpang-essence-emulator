#pragma once

#include "packets/LobbyServerPacket.h"

class SendGiftInShopError : public LobbyServerPacket
{
public:
	enum LS_SEND_PRESENT_IN_SHOP_FAIL
	{
		LACK_MONEY = 303,
		SOLD_OUT = 361,
		SEND_GIFT_FAIL_TARGET_INVENTORY_FULL = 362,
		SEND_GIFT_FAIL_TARGET_NOT_EXIST = 363, // and 365, 827
		GIFT_FAIL_SEND_TO_SELF = 846,
		// 872 inappropriate room name
	};

	explicit SendGiftInShopError(const uint32_t code) : LobbyServerPacket(817) // 817, = LS_SEND_PRESENT_IN_SHOP_FAIL
	{
		writeInt(code);
	}
};