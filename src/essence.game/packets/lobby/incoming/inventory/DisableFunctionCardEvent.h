#pragma once

#include "core/communication/packet/PacketEvent.h"
#include "packets/lobby/outgoing/inventory/DisableFunctionCard.h"
#include "qpang/ItemID.h"
#include "qpang/player/Player.h"
#include "qpang/player/inventory/InventoryManager.h"

class DisableFunctionCardEvent final : public PacketEvent
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

		if (const auto card = player->getInventoryManager()->get(cardId); isEquippableFunction(card.itemId))
		{
			player->getInventoryManager()->setCardActive(card.id, false);
		}
	}
};
