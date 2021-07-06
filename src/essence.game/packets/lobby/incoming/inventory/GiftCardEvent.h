#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/player/Player.h"
#include "qpang/Game.h"

#include "packets/lobby/outgoing/inventory/ReceiveGift.h"
#include "packets/lobby/outgoing/inventory/GiftCardSuccess.h"

class GiftCardEvent : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto player = conn->getPlayer();

		const auto nickname = packet.readStringTerminated(16);
		const auto cardId = packet.readLong();
		const auto idk = packet.readInt();

		auto card = player->getInventoryManager()->get(cardId);

		if (!card.isGiftable)
		{
			player->broadcast(u"This card may not be gifted.");

			return;
		}

		const auto targetPlayer = Game::instance()->getPlayer(nickname);

		if (targetPlayer == nullptr || !targetPlayer)
		{
			player->broadcast(u"This player does not exist.");

			return;
		}

		if (!targetPlayer->getInventoryManager()->hasSpace() || !targetPlayer->getInventoryManager()->hasGiftSpace())
		{
			player->broadcast(u"This player does not have enough space in their inventory to receive a gift."
				"\nA player may only have 5 unopened gifts at a time.");
			
			return;
		}

		const auto hasEquipped = player->getEquipmentManager()->hasEquipped(cardId);

		if (hasEquipped)
		{
			return;
		}

		player->getInventoryManager()->giftCard(card, targetPlayer);
	}
};