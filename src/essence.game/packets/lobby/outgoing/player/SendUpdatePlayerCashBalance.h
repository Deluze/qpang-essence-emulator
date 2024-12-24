#pragma once

#include <cstdint>

#include "packets/LobbyServerPacket.h"

class SendUpdatePlayerCashBalance : public LobbyServerPacket
{
public:
	explicit SendUpdatePlayerCashBalance(const uint32_t cash) : LobbyServerPacket(832)
	{
		writeInt(cash);
	}
};