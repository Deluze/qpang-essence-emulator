#pragma once

#include "packets/LobbyServerPacket.h"

class SendUseCraneFail : public LobbyServerPacket
{
public:
	enum LS_USE_CRANE_FAIL
	{
		INVALID_AMOUNT_OF_ATTEMPTS = 0x36f, // 879
		NOT_ENOUGH_COINS = 0x36e,			// 878
		INVENTORY_FULL = 0x16a,				// 362
	};

	SendUseCraneFail() : LobbyServerPacket(899)
	{
	}

	explicit SendUseCraneFail(const uint32_t code) : LobbyServerPacket(899)
	{
		writeInt(code);
	}
};
