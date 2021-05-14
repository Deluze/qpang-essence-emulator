#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"
#include "qpang/shop/ShopManager.h"

#include "packets/lobby/outgoing/shop/ShopItems.h"

class RequestShopItems : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		std::vector<ShopItem> items = Game::instance()->getShopManager()->list();

		conn->send(ShopItems(items));
	}
};