#pragma once

#include <cstdint>

#include "packets/LobbyServerPacket.h"

class UpdateCashBalance : public LobbyServerPacket
{
public:
	UpdateCashBalance(uint32_t cash) : LobbyServerPacket(832)
	{
		writeInt(cash);
	}
};