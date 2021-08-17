#pragma once

#include "core/communication/packet/PacketEvent.h"
#include "packets/lobby/outgoing/misc/UseCrainFail.h"
#include "packets/lobby/outgoing/misc/UseCraneSuccess.h"
#include "qpang/Game.h"
#include "qpang/player/Player.h"

class UseCraneEvent final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		const auto times = pack.readShort();

		const auto player = conn->getPlayer();

		if (player->getInventoryManager()->list().size() >= 200 - times)
		{
			player->broadcast(u"You have too many items in your inventory to be able to use the cranemachine.");

			return;
		}

		auto* crane = Game::instance()->getCraneManager();

		if (!crane->isEnabled())
		{
			player->broadcast(u"The cranemachine is currently out of use.");

			return;
		}

		uint32_t requiredCoins;

		switch (times)
		{
		case 1:
			requiredCoins = 200;
			break;
		case 3:
			requiredCoins = 500;
			break;
		case 7:
			requiredCoins = 1200;
			break;
		default:
			return;
		}

		if (requiredCoins > player->getCoins())
		{
			player->broadcast(u"You do not have the required amount of coins to use the crane machine.");

			return;
		}

		player->removeCoins(requiredCoins);

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
