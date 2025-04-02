#pragma once

#include "packets/LobbyServerPacket.h"

class SendReceiveGift : public LobbyServerPacket
{
public:
	SendReceiveGift(const InventoryCard& card, const std::u16string& sender) : LobbyServerPacket(821)
	{
		writeString(sender, 16);
		writeLong(card.id);
		writeFlag(true);  // ?
		writeLong(card.timeCreated);
	}
};