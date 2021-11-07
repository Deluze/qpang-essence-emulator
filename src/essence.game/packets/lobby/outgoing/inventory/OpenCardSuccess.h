#pragma once

#include "packets/LobbyServerPacket.h"

class OpenCardSuccess : public LobbyServerPacket
{
public:
	explicit OpenCardSuccess(const uint64_t cardId) : LobbyServerPacket(656)
	{
		writeLong(cardId);
	}
};
