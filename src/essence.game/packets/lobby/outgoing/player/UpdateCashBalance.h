#pragma once

#include <cstdint>

#include "packets/LobbyServerPacket.h"

class UpdateCashBalance : public LobbyServerPacket
{
public:
	explicit UpdateCashBalance(const uint32_t cash) : LobbyServerPacket(832)
	{
		writeInt(cash);
	}
};