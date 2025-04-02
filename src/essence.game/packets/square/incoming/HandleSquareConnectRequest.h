#pragma once

#include <string>

#include "core/communication/packet/PacketEvent.h"
#include "packets/square/outgoing/SendSquareList.h"
#include "qpang/Game.h"
#include "qpang/player/Player.h"

class HandleSquareConnectRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto playerId = packet.readInt();
		const auto nickname = packet.readString(16);

		const auto player = Game::instance()->getPlayer(playerId);

		if (player == nullptr)
		{
			return;
		}

		player->setSquareConn(conn);

		conn->setPlayer(player, playerId);

		const std::vector<Square::Ptr> squares = Game::instance()->getSquareManager()->list();

		conn->send(SendSquareList(squares));
	}
};
