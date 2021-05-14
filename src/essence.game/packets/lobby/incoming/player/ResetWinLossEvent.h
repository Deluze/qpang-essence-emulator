#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/player/Player.h"

#include "qpang/player/equipment/EquipmentManager.h"
#include "qpang/player/inventory/InventoryManager.h"

#include "qpang/ItemID.h"

#include "packets/lobby/outgoing/player/ResetWinLoss.h"

class ResetWinLossEvent : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		auto cardId = packet.readLong();

		auto player = conn->getPlayer();

		if (player == nullptr)
			return;

		auto card = player->getInventoryManager()->get(cardId);

		if (card.itemId != WL_CLEANER || card.period == 0)
			return;

		player->getStatsManager()->clearWL();

		if (card.periodType != 254)
			player->getInventoryManager()->deleteCard(cardId);

		conn->send(ResetWinLoss(player, card));
	}
};