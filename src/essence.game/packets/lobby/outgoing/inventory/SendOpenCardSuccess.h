#pragma once

#include "packets/LobbyServerPacket.h"

class SendOpenCardSuccess : public LobbyServerPacket
{
public:
	explicit SendOpenCardSuccess(const uint64_t cardId) : LobbyServerPacket(656)
	{
		writeLong(cardId);
	}
};
