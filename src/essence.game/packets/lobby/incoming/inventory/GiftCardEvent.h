#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"
#include "qpang/player/Player.h"

class GiftCardEvent final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto player = conn->getPlayer();

		const auto nickname = packet.readStringTerminated(16);
		const auto cardId = packet.readLong();
		// ReSharper disable once CppDeclaratorNeverUsed
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
			return;
		}

		if (!targetPlayer->getInventoryManager()->hasSpace() /*|| !targetPlayer->getInventoryManager()->hasGiftSpace()*/)
		{
			player->broadcast(u"The player you are trying to gift an item to does not have enough inventory space.");
			
			return;
		}

		if (player->getEquipmentManager()->hasEquipped(cardId))
		{
			return;
		}

		player->getInventoryManager()->giftCard(card, targetPlayer);
	}
};