#pragma once

#include "core/communication/packet/PacketEvent.h"
#include "qpang/player/Player.h"
#include "qpang/player/inventory/InventoryManager.h"

class HandleGetGiftsRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto player = conn->getPlayer();

		if (player == nullptr)
		{
			return;
		}

		player->getInventoryManager()->sendGifts();
	}
};
