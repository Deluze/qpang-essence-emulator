#pragma once

#include "core/communication/packet/PacketEvent.h"

class OpenCardRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto cardId = packet.readLong();
		const auto player = conn->getPlayer();

		if (const auto hasCard = player->getInventoryManager()->hasCard(cardId); !hasCard)
		{
			return;
		}

		player->getInventoryManager()->openCard(cardId);
	}
};
