#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/player/Player.h"

#include "qpang/player/equipment/EquipmentManager.h"
#include "qpang/player/inventory/InventoryManager.h"

#include "qpang/ItemID.h"

#include "packets/lobby/outgoing/inventory/EnableFunctionCard.h"

class EnableFunctionCardEvent : public PacketEvent
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
			return player->getInventoryManager()->setCardActive(card.id, true);
	}
};