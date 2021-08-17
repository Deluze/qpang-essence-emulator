#pragma once

#include "core/communication/packet/PacketEvent.h"
#include "packets/lobby/outgoing/inventory/Inventory.h"
#include "qpang/player/Player.h"
#include "qpang/player/inventory/InventoryManager.h"

class RequestInventory final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const std::vector<InventoryCard> cards = conn->getPlayer()->getInventoryManager()->list();

		conn->send(Inventory(cards));
	}
};
