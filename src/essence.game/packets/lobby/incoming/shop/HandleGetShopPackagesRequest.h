#pragma once

#include "core/communication/packet/PacketEvent.h"

class HandleGetShopPackagesRequest final : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		// TODO:
	}
};