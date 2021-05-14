#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"
#include "qpang/player/Player.h"

#include "packets/lobby/outgoing/player/PlayerInfoInspector.h"

class RequestPlayerInfo : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		auto playerId = pack.readInt();

		auto player = Game::instance()->getPlayer(playerId);

		if (player == nullptr)
			return;

		conn->send(PlayerInfoInspector(player));
	}
};