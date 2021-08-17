#pragma once

#include "packets/LobbyServerPacket.h"

class GiftCardSuccess : public LobbyServerPacket
{
public:
	explicit GiftCardSuccess(const uint64_t cardId) : LobbyServerPacket(813)
	{
		writeEmpty(34);
		writeLong(cardId);
	}
};