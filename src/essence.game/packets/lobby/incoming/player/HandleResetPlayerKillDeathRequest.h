#pragma once

#include "core/communication/packet/PacketEvent.h"
#include "packets/lobby/outgoing/player/SendResetPlayerKillDeath.h"
#include "qpang/ItemId.h"
#include "qpang/player/Player.h"
#include "qpang/player/inventory/InventoryManager.h"

class HandleResetPlayerKillDeathRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto cardId = packet.readLong();
		const auto player = conn->getPlayer();

		if (player == nullptr)
		{
			return;
		}

		auto card = player->getInventoryManager()->get(cardId);

		if (card.itemId != KD_CLEANER || card.period == 0)
		{
			return;
		}

		player->getStatsManager()->clearKD();

		if(card.periodType != 254) // Unlimited
		{
			player->getInventoryManager()->deleteCard(cardId);
		}
		
		conn->send(SendResetPlayerKillDeath(player, card));
	}
};
