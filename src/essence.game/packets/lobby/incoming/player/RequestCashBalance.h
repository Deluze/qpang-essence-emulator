#pragma once

#include <cstdint>

#include "core/communication/packet/PacketEvent.h"

#include "qpang/player/Player.h"
#include "packets/lobby/outgoing/player/UpdateCashBalance.h"

class RequestCashBalance : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		uint32_t cash = conn->getPlayer()->getCash();

		conn->send(UpdateCashBalance(cash));
	}
};