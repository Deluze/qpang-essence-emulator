#pragma once

#include <cstdint>
#include <string>
#include <exception>

#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"
#include "qpang/player/Player.h"

#include "packets/square/outgoing/SquareList.h"

class ConnectRequest : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto playerId = packet.readInt();
		const auto nickname = packet.readString(16);

		auto player = Game::instance()->getPlayer(playerId);

		if (player == nullptr)
			return;

		player->setSquareConn(conn);
		conn->setPlayer(player);

		std::vector<Square::Ptr> squares = Game::instance()->getSquareManager()->list();
		conn->send(SquareList(squares));
	}
};