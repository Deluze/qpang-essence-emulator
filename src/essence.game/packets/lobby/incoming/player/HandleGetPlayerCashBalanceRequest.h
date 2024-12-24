#pragma once

#include <cstdint>

#include "core/communication/packet/PacketEvent.h"
#include "packets/lobby/outgoing/player/SendUpdatePlayerCashBalance.h"
#include "qpang/player/Player.h"

class HandleGetPlayerCashBalanceRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const uint32_t cash = conn->getPlayer()->getCash();

		conn->send(SendUpdatePlayerCashBalance(cash));
	}
};
