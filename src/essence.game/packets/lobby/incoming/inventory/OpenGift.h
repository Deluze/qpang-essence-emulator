#pragma once

#include "core/communication/packet/PacketEvent.h"
#include "qpang/player/Player.h"

class OpenGift final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto cardId = packet.readLong();

		conn->getPlayer()->getInventoryManager()->openGift(cardId);
	}
};
