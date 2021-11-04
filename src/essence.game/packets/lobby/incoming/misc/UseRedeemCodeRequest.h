#pragma once

#include "Player.h"
#include "UseRedeemCodeFailResponse.h"
#include "UseRedeemCodeSuccessResponse.h"
#include "core/communication/packet/PacketEvent.h"

class UseRedeemCodeRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		const auto player = conn->getPlayer();

		std::cout << "UseRedeemCodeRequest::handle >> Player " << player->getId() << " has used a redeem code" << std::endl;

		const auto redeemCodeChars = pack.readArray<char, 14>();
		const auto redeemCode = std::string(redeemCodeChars.cbegin(), redeemCodeChars.cend());

		std::cout << "UseRedeemCodeRequest::handle >> Redeem code: " << redeemCode << std::endl;

		// TODO: Check if redeem code is valid, and if so, give item?

		const std::vector<InventoryCard> inventoryCards = {};

		// TODO: For now, send a fail response since it's not fully done.
		conn->send(UseRedeemCodeFailResponse());
		//conn->send(UseRedeemCodeSuccessResponse(player->getDon(), player->getCash(), inventoryCards));
	}
};
