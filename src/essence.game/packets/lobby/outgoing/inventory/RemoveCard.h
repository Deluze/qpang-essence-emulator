#pragma once

#include "packets/LobbyServerPacket.h"

class RemoveCard : public LobbyServerPacket
{
public:
	RemoveCard(const uint64_t cardId) : LobbyServerPacket(653)
	{
		writeLong(cardId);
	}
};