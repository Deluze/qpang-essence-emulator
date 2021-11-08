#pragma once

#include "packets/LobbyServerPacket.h"

class SendPresentInShopSuccess : public LobbyServerPacket
{
public:
	SendPresentInShopSuccess(const bool isCash, const uint32_t newBalance)
		: LobbyServerPacket(816) // 816 = success, 817 = LS_SEND_PRESENT_IN_SHOP_FAIL
	{
		writeEmpty(34);

		writeInt(newBalance); // 34

		writeEmpty(8);

		writeFlag(isCash); // 46
	}
};

