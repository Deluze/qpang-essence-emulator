#pragma once

#include "packets/LobbyServerPacket.h"

class SendGiftInventoryCardSuccess : public LobbyServerPacket
{
public:
	explicit SendGiftInventoryCardSuccess(const uint64_t cardId) : LobbyServerPacket(813)
	{
		writeEmpty(34);
		writeLong(cardId);
	}
};