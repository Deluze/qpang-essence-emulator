#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"
#include "qpang/player/Player.h"

#include "packets/lobby/outgoing/misc/UseCrainFail.h"
#include "packets/lobby/outgoing/misc/UseCraneSuccess.h"

class UseCraneEvent : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		const auto times = pack.readShort();

		auto player = conn->getPlayer();

		if (player->getInventoryManager()->list().size() >= 200 - times)
		{
			
			return;
		}

		auto* crane = Game::instance()->getCraneManager();

		if (!crane->isEnabled())
		{
			player->broadcast(u"Tell the developers to put something in this crane ;)");
			return;
		}

		uint32_t coinsNeeded = 0;

		switch (times)
		{
		case 1:
			coinsNeeded = 200;
			break;
		case 3:
			coinsNeeded = 500;
			break;
		case 7:
			coinsNeeded = 1200;
			break;
		default:
			return;
		}

		if (coinsNeeded > player->getCoins())
			return;

		player->removeCoins(coinsNeeded);

		std::vector<InventoryCard> cards;
		
		for (size_t i = 0; i < times; i++)
		{
			auto inventoryCard = crane->getRandomItem();
			inventoryCard.playerOwnerId = player->getId();
			inventoryCard.isOpened = true;
			player->getInventoryManager()->storeCard(inventoryCard);
			
			cards.push_back(inventoryCard);
		}

		player->send(UseCraneSuccess(player, cards));
	}
};