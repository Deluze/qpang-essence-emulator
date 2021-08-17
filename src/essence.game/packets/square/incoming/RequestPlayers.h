#pragma once

#include <vector>

#include "core/communication/packet/PacketEvent.h"
#include "packets/square/outgoing/Players.h"
#include "qpang/square/SquarePlayer.h"

class RequestPlayers final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto player = conn->getPlayer();

		if (player == nullptr)
		{
			return;
		}

		const auto squarePlayer = player->getSquarePlayer();

		if (squarePlayer == nullptr)
		{
			return;
		}

		const auto square = squarePlayer->getSquare();
		const auto players = square->listPlayers();

		conn->send(Players(players, player->getId()));
	}
};
