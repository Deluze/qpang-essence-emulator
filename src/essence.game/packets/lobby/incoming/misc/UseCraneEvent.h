#pragma once

#include "UseCraneFailResponse.h"
#include "core/communication/packet/PacketEvent.h"
#include "packets/lobby/outgoing/misc/UseCraneSuccessResponse.h"
#include "qpang/Game.h"
#include "qpang/player/Player.h"

class UseCraneEvent final : public PacketEvent
{
public:
	enum LS_USE_CRANE_FAIL
	{
		INVALID_AMOUNT_OF_ATTEMPTS = 0x36f, // 879
		NOT_ENOUGH_COINS = 0x36e,			// 878
		INVENTORY_FULL = 0x16a,				// 362
	};

	void handle(const QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		const auto player = conn->getPlayer();

		const auto times = pack.readShort();

		if (!player->getInventoryManager()->hasSpace(times))
		{
			conn->send(UseCraneFailResponse(INVENTORY_FULL));

			return;
		}

		auto* crane = Game::instance()->getCraneManager();

		if (!crane->isEnabled())
		{
			conn->send(UseCraneFailResponse());

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
			conn->send(UseCraneFailResponse(INVALID_AMOUNT_OF_ATTEMPTS));

			return;
		}

		if (requiredCoins > player->getCoins())
		{
			conn->send(UseCraneFailResponse(NOT_ENOUGH_COINS));

			return;
		}

		player->removeCoins(requiredCoins);

		std::vector<InventoryCard> cards;

		for (size_t i = 0; i < times; i++)
		{
			auto inventoryCard = crane->getRandomItem();

			inventoryCard.playerOwnerId = player->getId();
			inventoryCard.isOpened = true;

			const auto createdCard = player->getInventoryManager()->storeCard(inventoryCard);

			cards.push_back(createdCard);
		}

		conn->send(UseCraneSuccessResponse(player, cards));
	}
};
