#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/player/Player.h"

#include "qpang/player/equipment/EquipmentManager.h"
#include "qpang/player/inventory/InventoryManager.h"

#include "qpang/ItemID.h"

#include "packets/lobby/outgoing/inventory/DisableFunctionCard.h"

class DisableFunctionCardEvent : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet)
	{
		auto cardId = packet.readLong();

		auto player = conn->getPlayer();

		if (player == nullptr)
			return;

		auto card = player->getInventoryManager()->get(cardId);

		if (isEquippableFunction(card.itemId))
			player->getInventoryManager()->setCardActive(card.id, false);
	}
};