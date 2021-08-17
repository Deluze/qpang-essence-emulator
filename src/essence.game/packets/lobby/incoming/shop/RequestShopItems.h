#pragma once

#include "core/communication/packet/PacketEvent.h"
#include "packets/lobby/outgoing/shop/ShopItems.h"
#include "qpang/Game.h"
#include "qpang/shop/ShopManager.h"

class RequestShopItems final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		const std::vector<ShopItem> items = Game::instance()->getShopManager()->list();

		conn->send(ShopItems(items));
	}
};
