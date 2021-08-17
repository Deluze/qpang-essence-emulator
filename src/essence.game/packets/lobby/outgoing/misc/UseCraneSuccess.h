#pragma once

#include "packets/LobbyServerPacket.h"
#include "packets/writers/InventoryCardWriter.h"
#include "qpang/player/inventory/InventoryCard.h"

class UseCraneSuccess: public LobbyServerPacket
{
public:
	UseCraneSuccess(const std::shared_ptr<Player>& player, const std::vector<InventoryCard>& cards) : LobbyServerPacket(898)
	{
		writeShort(cards.size());
		writeInt(player->getDon());
		writeInt(player->getCoins());

		for (size_t i = 0; i < cards.size(); i++)
		{
			writeInt(cards[i].itemId);
			writeEmpty(5);
			InventoryCardWriter::write(this, cards[i]);
		}
	}
};
