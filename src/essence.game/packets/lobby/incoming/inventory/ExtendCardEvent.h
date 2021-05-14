#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/player/Player.h"
#include "qpang/player/inventory/InventoryManager.h"

#include "packets/lobby/outgoing/inventory/CardExtended.h"

class ExtendCardEvent : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet)
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