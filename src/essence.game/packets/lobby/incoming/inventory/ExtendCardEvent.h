#pragma once

#include "core/communication/packet/PacketEvent.h"

class ExtendCardEvent final : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		auto cardId = packet.readLong();
		auto itemId = packet.readInt();
		auto idk2 = packet.readFlag();
		auto price = packet.readInt();
		auto seqId = packet.readInt();
		auto flag = packet.readFlag();
		auto kek = packet.readShort();

		//auto card = conn->getPlayer()->getInventoryManager()->get(cardId);
		//
		//card.period = 10;

		//conn->send(CardExtended(card, 100, true));
	}
};	