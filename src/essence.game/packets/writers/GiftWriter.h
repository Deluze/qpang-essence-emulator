#pragma once

#include "core/communication/packet/ServerPacket.h"
#include "qpang/player/inventory/InventoryCard.h"

class GiftWriter
{
public:
	static void write(ServerPacket* packet, const InventoryCard& card)
	{
		packet->writeString(u"--", 16); // sender name
		packet->writeLong(card.id);
		packet->writeFlag(true);
		packet->writeInt(card.timeCreated);
	}
};
