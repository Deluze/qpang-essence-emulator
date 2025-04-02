#pragma once

#include "core/communication/packet/PacketEvent.h"
#include "qpang/player/Player.h"
#include "qpang/player/inventory/InventoryManager.h"

class HandleGetInventoryRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		conn->getPlayer()->getInventoryManager()->sendCards();
	}
};
