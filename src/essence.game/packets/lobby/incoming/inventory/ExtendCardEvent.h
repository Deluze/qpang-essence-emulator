#pragma once

#include "CardExtended.h"
#include "CardPurchaseComplete.h"
#include "OpenGiftSuccess.h"
#include "UpdateCashBalance.h"
#include "core/communication/packet/PacketEvent.h"

class ExtendCardEvent final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto cardId = packet.readLong();
		const auto itemId = packet.readInt();

		const auto payType = packet.readFlag() ? 1 : 0; // Paytype (don or cash), true = cash, false = don;
		const auto isCash = payType == 1;

		const auto price = packet.readInt();
		const auto seqId = packet.readInt();

		const auto unkFlag1 = packet.readFlag();
		const auto unkShort1 = packet.readShort();

		const auto player = conn->getPlayer();

		// Ensure the player exists.
		if (player == nullptr)
		{
			return;
		}

		const auto squarePlayer = player->getSquarePlayer();

		if (squarePlayer == nullptr)
		{
			player->broadcast(u"You may only renew items from the park.");

			return;
		}

		const auto playerHasCard = player->getInventoryManager()->hasCard(cardId);
		const auto cardIsExpired = player->getInventoryManager()->isExpired(cardId);
		const auto cardIsEquipped = player->getEquipmentManager()->hasEquipped(cardId);

		if (!playerHasCard || !cardIsExpired || cardIsEquipped)
		{
			return;
		}

		// You may only extend a card if it currently exists in the shop, so cancel if it does not exist.
		if (!Game::instance()->getShopManager()->exists(seqId))
		{
			return;
		}

		const auto shopItem = Game::instance()->getShopManager()->get(seqId);

		if (shopItem.soldCount >= shopItem.stock && shopItem.stock != 9999)
		{
			player->broadcast(u"You may not renew this item since it is out of stock.");

			return;
		}

		// Validate that the values from the packet are the same as the shop in the database.
		if ((shopItem.isCash != isCash) || (shopItem.itemId != itemId) || (shopItem.price != price))
		{
			return;
		}

		// Ensure the player has sufficient funds to pay for the item.
		if (const auto hasSufficientFunds = (shopItem.isCash)
			? player->getCash() >= shopItem.price
			: player->getDon() >= shopItem.price; !hasSufficientFunds)
		{
			return;
		}

		player->getInventoryManager()->deleteCard(cardId);

		const auto boughtInventoryCard = Game::instance()->getShopManager()->buy(player, seqId, false);

		conn->send(CardExtended(boughtInventoryCard, shopItem.isCash ? player->getCash() : player->getDon(), isCash));
		conn->send(UpdateCashBalance(player->getCash()));

		player->send(OpenGiftSuccess(player, boughtInventoryCard));
	}
};