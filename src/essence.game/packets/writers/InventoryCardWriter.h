#pragma once

#include "core/communication/packet/ServerPacket.h"
#include "qpang/player/inventory/InventoryCard.h"

class InventoryCardWriter
{
public:
	static void write(ServerPacket* packet, const InventoryCard& card)
	{
		packet->writeLong(card.id); // 0
		packet->writeInt(card.itemId); // 8
		packet->writeByte(10); // 12
		packet->writeByte(card.type); // 13
		packet->writeByte(0); // 14
		packet->writeFlag(card.isGiftable); // 15
		packet->writeEmpty(6); // 16
		packet->writeInt(card.timeCreated); // 22
		packet->writeFlag(card.isOpened); // 26
		packet->writeShort(card.isActive ? 0 : 1); // if 1, function card de-quipped. if 0 equipped?????? // 27
		packet->writeFlag(false); // hidden // 28
		packet->writeFlag(false);
		packet->writeShort(card.period);
		packet->writeByte(card.periodType);
		packet->writeFlag(false);
		packet->writeShort(card.boostLevel);
		packet->writeFlag(card.boostLevel > 0);
		packet->writeFlag(false);
		packet->writeEmpty(4);
	}
};
