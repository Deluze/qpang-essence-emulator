#pragma once

#include "packets/LobbyServerPacket.h"

class SendPresentInShopSuccess : public LobbyServerPacket
{
public:
	SendPresentInShopSuccess() : LobbyServerPacket(816) // 816 = success, 817 = LS_SEND_PRESENT_IN_SHOP_FAIL
	{
	}
};
