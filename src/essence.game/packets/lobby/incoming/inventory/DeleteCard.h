#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/player/Player.h"
#include "qpang/player/inventory/InventoryManager.h"

class DeleteCard : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet)
	{
		const auto cardId = packet.readLong();

		const auto player = conn->getPlayer();

		if (player == nullptr)
			return;

		player->getInventoryManager()->deleteCard(cardId);
	}
};