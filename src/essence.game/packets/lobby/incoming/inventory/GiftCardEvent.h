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

			return;
		}
		
		const auto targetPlayer = Game::instance()->getPlayer(nickname);

		if (targetPlayer == nullptr)
		{
		
			return;
		}

		if (!targetPlayer->getInventoryManager()->hasSpace() || !targetPlayer->getInventoryManager()->hasGiftSpace())
		{
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