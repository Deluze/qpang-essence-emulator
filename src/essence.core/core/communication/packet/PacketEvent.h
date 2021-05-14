#pragma once

#include <iostream>

#include "core/communication/client/QpangConnection.h"
#include "core/communication/packet/QpangPacket.h"

class PacketEvent
{
public:
	virtual bool isUnauthorizedPacket()
	{
		return false;
	}

	virtual void handle(QpangConnection::Ptr client, QpangPacket& packet)
	{
		return;
	}
};