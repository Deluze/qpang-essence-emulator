#pragma once

#include <vector>
#include <algorithm>

#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"
#include "qpang/square/SquareManager.h"
#include "qpang/square/SquarePlayer.h"

#include "packets/square/outgoing/Players.h"

class RequestPlayers : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet)
	{
		auto player = conn->getPlayer();

		if (player == nullptr)
			return;

		auto squarePlayer = player->getSquarePlayer();

		if (squarePlayer == nullptr)
			return;

		const auto square = squarePlayer->getSquare();
		const auto players = square->listPlayers();
		conn->send(Players(players, player->getId()));
	}
};