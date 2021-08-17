#pragma once

#include "core/communication/packet/PacketEvent.h"
#include "packets/lobby/outgoing/inventory/Gifts.h"
#include "qpang/player/Player.h"
#include "qpang/player/inventory/InventoryManager.h"

class RequestGifts final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto player = conn->getPlayer();

		if (player == nullptr)
		{
			return;
		}

		const auto& gifts = player->getInventoryManager()->listGifts();

		conn->send(Gifts(gifts));
	}
};
