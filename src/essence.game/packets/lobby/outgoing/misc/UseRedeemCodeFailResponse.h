#pragma once

#include "packets/LobbyServerPacket.h"

class UseRedeemCodeFailResponse : public LobbyServerPacket
{
public:
	explicit UseRedeemCodeFailResponse() : LobbyServerPacket(853) // 853
	{
	
	}
};