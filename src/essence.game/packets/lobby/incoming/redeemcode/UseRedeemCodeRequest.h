#pragma once

#include "Player.h"
#include "RedeemCodeManager.h"
#include "UseRedeemCodeFailResponse.h"
#include "core/communication/packet/PacketEvent.h"

class UseRedeemCodeRequest final : public PacketEvent
{
public:
	// TODO: Find out error codes for redeem failure.
	void handle(const QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		const auto player = conn->getPlayer();
		const auto playerId = player->getId();

		const auto redeemCodeChars = pack.readArray<char, 14>();
		const auto code = std::u16string(redeemCodeChars.cbegin(), redeemCodeChars.cend());

		if (!RedeemCodeManager::isOfValidLength(code))
		{
			conn->send(UseRedeemCodeFailResponse());

			return;
		}

		const auto redeemCode = RedeemCodeManager::tryGetRedeemCodeForPlayer(code, playerId);

		if (redeemCode.id == 0)
		{
			conn->send(UseRedeemCodeFailResponse());

			return;
		}

		const auto inventoryCards = RedeemCodeManager::getInventoryCardsForRedeemCode(redeemCode.id);

		if (!inventoryCards.empty() && !player->getInventoryManager()->hasSpace(inventoryCards.size()))
		{
			conn->send(UseRedeemCodeFailResponse());

			player->broadcast(u"You do not have enough inventory space left to redeem this code.");

			return;
		}

		RedeemCodeManager::sendRedeemCodeRewardsToPlayer(player, redeemCode, inventoryCards);
	}
};