#pragma once

#include "SendRenewInventoryCardSuccess.h"
#include "SendShopCardPurchaseComplete.h"
#include "SendOpenGiftSuccess.h"
#include "SendUpdatePlayerCashBalance.h"
#include "SendGiftInShopError.h"
#include "SendGiftInventoryCardFail.h"
#include "core/communication/packet/PacketEvent.h"

class HandleRenewInventoryCardRequest final : public PacketEvent
{
	class Send811 : public LobbyServerPacket
	{
	public:
		Send811(const uint32_t errorMessageCode) : LobbyServerPacket(811)
		{
			writeInt(errorMessageCode);
		}
	};

public:
	enum LS_RENEW_CARD_FAIL {
		DURATION_CAN_NOT_BE_EXTENDED = 351
	};

	// TODO: Find out error response op code and error message codes.
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto cardId = packet.readLong();
		const auto itemId = packet.readInt();

		const auto payType = packet.readByte();
		const auto isCash = (payType == 1);

		const auto price = packet.readInt();
		const auto seqId = packet.readInt();
		
		// Remaining 3 bytes are unknown.

		const auto player = conn->getPlayer();

		const auto hasCard = player->getInventoryManager()->hasCard(cardId);
		const auto isCardExpired = player->getInventoryManager()->isExpired(cardId);
		const auto hasCardEquipped = player->getEquipmentManager()->hasEquipped(cardId);

		if (!hasCard || !isCardExpired || hasCardEquipped)
		{
			conn->send(Send811(DURATION_CAN_NOT_BE_EXTENDED));

			return;
		}

		// Only card that are in the current shop may be renewed.
		if (!Game::instance()->getShopManager()->exists(seqId))
		{
			conn->send(Send811(DURATION_CAN_NOT_BE_EXTENDED));

			return;
		}

		const auto shopItem = Game::instance()->getShopManager()->get(seqId);

		if (shopItem.soldCount >= shopItem.stock && shopItem.stock != 9999)
		{
			conn->send(SendGiftInShopError(SendGiftInShopError::LS_SEND_PRESENT_IN_SHOP_FAIL::SOLD_OUT));

			return;
		}

		// Validate that the values from the packet are the same as the shop in the database.
		if ((shopItem.isCash != isCash) || (shopItem.itemId != itemId) || (shopItem.price != price))
		{
			conn->send(Send811(DURATION_CAN_NOT_BE_EXTENDED));

			return;
		}

		// Ensure the player has sufficient funds to pay for the item.
		if (const auto hasSufficientFunds = (shopItem.isCash)
			? player->getCash() >= shopItem.price
			: player->getDon() >= shopItem.price; !hasSufficientFunds)
		{
			conn->send(SendGiftInShopError(SendGiftInShopError::LS_SEND_PRESENT_IN_SHOP_FAIL::LACK_MONEY));

			return;
		}

		player->getInventoryManager()->deleteCard(cardId);

		const auto boughtInventoryCard = Game::instance()->getShopManager()->buy(player, seqId, false);

		conn->send(SendRenewInventoryCardSuccess(boughtInventoryCard, shopItem.isCash ? player->getCash() : player->getDon(), isCash));
		conn->send(SendUpdatePlayerCashBalance(player->getCash()));
		conn->send(SendOpenGiftSuccess(player, boughtInventoryCard));
	}
};