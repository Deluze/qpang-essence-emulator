#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"
#include "qpang/shop/ShopManager.h"

#include "packets/lobby/outgoing/shop/ShopItems.h"

class RequestShopPackages : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		// TODO:
	}
};