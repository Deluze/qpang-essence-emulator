#pragma once

#include "packets/LobbyServerPacket.h"
#include "packets/writers/InventoryCardWriter.h"
#include "qpang/player/inventory/InventoryCard.h"

class SendUseCraneSuccess : public LobbyServerPacket
{
public:
	SendUseCraneSuccess(const std::shared_ptr<Player>& player, const std::vector<InventoryCard>& cards) : LobbyServerPacket(898)
	{
		writeShort(cards.size());

		writeInt(player->getDon());
		writeInt(player->getCoins());

		for (size_t i = 0; i < cards.size(); i++)
		{
			writeInt(cards[i].itemId);					// 0 = fail
			writeEmpty(5);
			InventoryCardWriter::write(this, cards[i]); // writeEmpty(43);
		}
	}
};