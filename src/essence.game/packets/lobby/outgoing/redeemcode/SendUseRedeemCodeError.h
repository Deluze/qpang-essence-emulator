#pragma once

#include "packets/LobbyServerPacket.h"

class SendUseRedeemCodeError : public LobbyServerPacket
{
public:
	explicit SendUseRedeemCodeError(const uint32_t code) : LobbyServerPacket(853) // 853
	{
		writeInt(code);
	}
};