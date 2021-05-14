#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/player/Player.h"
#include "qpang/player/inventory/InventoryManager.h"

#include "packets/lobby/outgoing/inventory/Inventory.h"
#include "packets/lobby/outgoing/inventory/CardExtended.h"

class RequestInventory : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		std::vector<InventoryCard> cards = conn->getPlayer()->getInventoryManager()->list();

		conn->send(Inventory(cards));
	}
};