#pragma once

#include "core/communication/packet/PacketEvent.h"
#include <packets/lobby/outgoing/misc/DailyBonusResponse.h>

class RequestDailyBonus final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		const auto player = conn->getPlayer();

		player->send(DailyBonusResponse(player, false));
	}
};
