#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/player/Player.h"
#include "qpang/player/inventory/InventoryManager.h"

#include "packets/lobby/outgoing/inventory/Gifts.h"

class RequestGifts : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet)
	{
		const auto player = conn->getPlayer();

		if (player == nullptr)
			return;

		const auto& gifts = player->getInventoryManager()->listGifts();

		conn->send(Gifts(gifts));
	}
};