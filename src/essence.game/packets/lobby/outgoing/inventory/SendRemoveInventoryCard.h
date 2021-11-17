#pragma once

#include "packets/LobbyServerPacket.h"

class SendRemoveInventoryCard : public LobbyServerPacket
{
public:
	explicit SendRemoveInventoryCard(const uint64_t cardId) : LobbyServerPacket(653)
	{
		writeLong(cardId);
	}
};