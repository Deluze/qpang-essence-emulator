#pragma once

#include "packets/LobbyServerPacket.h"

class GiftCardSuccess : public LobbyServerPacket
{
public:
	GiftCardSuccess(const uint64_t cardId) : LobbyServerPacket(813)
	{
		writeEmpty(34);
		writeLong(cardId);
	}
};