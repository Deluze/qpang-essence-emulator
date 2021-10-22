#pragma once

#include "core/communication/packet/PacketEvent.h"
#include "packets/lobby/outgoing/inventory/EnableFunctionCard.h"
#include "qpang/ItemId.h"
#include "qpang/player/Player.h"
#include "qpang/player/inventory/InventoryManager.h"

class EnableFunctionCardEvent final : public PacketEvent
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
			return player->getInventoryManager()->setCardActive(card.id, true);
		}
	}
};
