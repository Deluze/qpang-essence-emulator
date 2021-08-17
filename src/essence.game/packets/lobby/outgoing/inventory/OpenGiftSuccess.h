#pragma once

#include "packets/LobbyServerPacket.h"
#include "packets/writers/InventoryCardWriter.h"
#include "qpang/player/inventory/InventoryCard.h"

class OpenGiftSuccess : public LobbyServerPacket
{
public:
	OpenGiftSuccess(const std::shared_ptr<Player>& player, const InventoryCard& card) : LobbyServerPacket(743)
	{
		writeInt(static_cast<uint32_t>(card.id));
		writeInt(player->getDon());

		writeByte(1); // card count

		InventoryCardWriter::write(this, card);
	}
};
