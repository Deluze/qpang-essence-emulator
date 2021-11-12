#pragma once

#include "packets/LobbyServerPacket.h"

class UseRedeemCodeFailResponse : public LobbyServerPacket
{
public:
	explicit UseRedeemCodeFailResponse(const uint32_t code) : LobbyServerPacket(853) // 853
	{
		writeInt(code);
	}
};