#pragma once

#include "SendGiftInShopError.h"
#include "SendGiftInShopSuccess.h"
#include "core/communication/packet/PacketEvent.h"

class RequestSendPresentInShop final : public PacketEvent
{
public:
	enum LS_SEND_PRESENT_IN_SHOP_FAIL
	{
		LACK_MONEY = 303,
		SOLD_OUT = 361,
		SEND_GIFT_FAIL_TARGET_INVENTORY_FULL = 362,
		SEND_GIFT_FAIL_TARGET_NOT_EXIST = 363, // and 365, 827
		GIFT_FAIL_SEND_TO_SELF = 846,
	};

	void handle(const QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		const auto buyingPlayer = conn->getPlayer();

		const auto targetNickName = pack.readStringTerminated(16);
		const auto targetPlayer = Game::instance()->getPlayer(targetNickName);

		pack.readEmpty(4);

		const auto seqId = pack.readInt();

		pack.readEmpty(5);

		const auto price = pack.readShort();

		// Target player does not exist.
		if (targetPlayer == nullptr)
		{
			conn->send(SendGiftInShopError(SEND_GIFT_FAIL_TARGET_NOT_EXIST));

			return;
		}

		// Check if the target player is the same as buying player.
		if (buyingPlayer->getId() == targetPlayer->getId())
		{
			conn->send(SendGiftInShopError(GIFT_FAIL_SEND_TO_SELF));

			return;
		}

		// Check if the shop item exists.
		if (!Game::instance()->getShopManager()->exists(seqId))
		{
			conn->send(SendGiftInShopError(SOLD_OUT));

			return;
		}

		// TODO: Check if shop item is valid/exists.
		const auto shopItem = Game::instance()->getShopManager()->get(seqId);

		if (shopItem.price != price)
		{
			// If we get here, the buying player has tampered with their amount of don/cash
			// or they have a shop item opened that no longer exists or which price has changed.

			conn->send(SendGiftInShopError(LACK_MONEY));

			return;
		}

		if (shopItem.soldCount >= shopItem.stock && shopItem.stock != 9999)
		{
			conn->send(SendGiftInShopError(SOLD_OUT));

			return;
		}

		// Check if buying player has sufficient funds.
		if (const auto buyerHasSufficientFunds = (shopItem.isCash)
			? (buyingPlayer->getCash() >= shopItem.price)
			: (buyingPlayer->getDon() >= shopItem.price); !buyerHasSufficientFunds)
		{
			conn->send(SendGiftInShopError(LACK_MONEY));

			return;
		}

		// Make sure the target player has space.
		if (!targetPlayer->getInventoryManager()->hasSpace())
		{
			conn->send(SendGiftInShopError(SEND_GIFT_FAIL_TARGET_INVENTORY_FULL));

			return;
		}

		uint32_t newBalance;

		// Remove the funds from the buying player.
		if (shopItem.isCash)
		{
			buyingPlayer->removeCash(shopItem.price);
			newBalance = buyingPlayer->getCash();
		}
		else
		{
			buyingPlayer->removeDon(shopItem.price);
			newBalance = buyingPlayer->getDon();
		}

		// Create the card from shop item.
		auto inventoryCard = InventoryCard::fromShopItem(shopItem);

		inventoryCard.isOpened = false;

		// Store the card in the database and add it to the target player.
		auto createdInventoryCard = targetPlayer->getInventoryManager()->storeCard(inventoryCard);

		// Send target player their gift
		targetPlayer->getInventoryManager()->receiveGift(createdInventoryCard, buyingPlayer->getName());

		// Send buying player success.
		conn->send(SendGiftInShopSuccess(shopItem.isCash, newBalance));
	}
};