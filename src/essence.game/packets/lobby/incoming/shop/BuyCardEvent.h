#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"
#include "qpang/shop/ShopManager.h"

class BuyCardEvent : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		auto seqId = pack.readInt();

		if (const auto player = conn->getPlayer(); player != nullptr)
			Game::instance()->getShopManager()->buy(player, seqId);
	}
};