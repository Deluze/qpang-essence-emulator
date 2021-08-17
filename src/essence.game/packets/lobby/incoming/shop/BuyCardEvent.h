#pragma once

#include "core/communication/packet/PacketEvent.h"
#include "qpang/Game.h"
#include "qpang/shop/ShopManager.h"

class BuyCardEvent final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		const auto seqId = pack.readInt();

		if (const auto player = conn->getPlayer(); player != nullptr)
		{
			Game::instance()->getShopManager()->buy(player, seqId, true);
		}
	}
};
