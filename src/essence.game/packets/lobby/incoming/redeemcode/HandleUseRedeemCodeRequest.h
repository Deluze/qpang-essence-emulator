#pragma once

#include "Player.h"
#include "RedeemCodeManager.h"
#include "core/communication/packet/PacketEvent.h"

class HandleUseRedeemCodeRequest final : public PacketEvent
{
public:
	enum USE_REDEEM_CODE_FAIL_CODE
	{
		DEFAULT = 0,
		NUMBER_HAS_ALREADY_BEEN_REGISTERED = 370,
		ALREADY_RECEIVED_DAILY_EVENT_ITEM = 374,
		PERIOD_OF_PINCODE_HAS_EXPIRED = 375,
		INVENTORY_IS_FULL = 840,
	};

	void handle(const QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		const auto player = conn->getPlayer();
		const auto playerId = player->getId();

		const auto redeemCodeChars = pack.readArray<char, 14>();
		const auto code = std::u16string(redeemCodeChars.cbegin(), redeemCodeChars.cend());

		if (!RedeemCodeManager::isOfValidLength(code))
		{
			RedeemCodeManager::sendUseRedeemCodeFailToPlayer(player, DEFAULT);

			return;
		}

		const auto redeemCode = RedeemCodeManager::tryGetRedeemCodeForPlayer(code, playerId);

		// Struct is not initialized.
		if (redeemCode.id == 0)
		{
			RedeemCodeManager::sendUseRedeemCodeFailToPlayer(player, NUMBER_HAS_ALREADY_BEEN_REGISTERED);

			return;
		}

		const auto inventoryCards = RedeemCodeManager::getInventoryCardsForRedeemCode(redeemCode.id);

		if (!inventoryCards.empty() && !player->getInventoryManager()->hasSpace(inventoryCards.size()))
		{
			RedeemCodeManager::sendUseRedeemCodeFailToPlayer(player, INVENTORY_IS_FULL);
			
			return;
		}

		RedeemCodeManager::sendRedeemCodeRewardsToPlayer(player, redeemCode, inventoryCards);
	}
};