#pragma once

#include "core/communication/packet/PacketEvent.h"
#include "qpang/Game.h"
#include "qpang/shop/ShopManager.h"

class RequestShopItems final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		Game::instance()->getShopManager()->sendShopItems(conn->getPlayer());
	}
};