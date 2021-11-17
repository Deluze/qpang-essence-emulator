#pragma once

#include "core/communication/packet/PacketEvent.h"
#include "packets/lobby/outgoing/player/SendPlayerInfoInspector.h"
#include "qpang/Game.h"

class RequestPlayerInfo final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		const auto playerId = pack.readInt();
		const auto player = Game::instance()->getPlayer(playerId);

		if (player == nullptr)
		{
			return;
		}

		conn->send(SendPlayerInfoInspector(player));
	}
};
