#pragma once

#include "GiftWriter.h"
#include "InventoryCardWriter.h"
#include "packets/LobbyServerPacket.h"

class UseRedeemCodeSuccessResponse : public LobbyServerPacket
{
public:
	explicit UseRedeemCodeSuccessResponse(
		const uint32_t donBalance,
		const uint32_t cashBalance,
		const std::vector<InventoryCard>& inventoryCards) : LobbyServerPacket(852) // 852
	{
		writeEmpty(8);

		writeInt(donBalance);
		writeInt(cashBalance);

		writeByte(inventoryCards.size()); // Amount of cards.

		for (const auto& inventoryCard : inventoryCards)
		{
			InventoryCardWriter::write(this, inventoryCard);
		}
	}
};