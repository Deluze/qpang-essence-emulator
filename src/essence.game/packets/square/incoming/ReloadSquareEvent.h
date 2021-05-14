#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/player/Player.h"
#include "qpang/square/Square.h"
#include "qpang/square/SquarePlayer.h"

#include "packets/square/outgoing/SetPosition.h"

class ReloadSquareEvent : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet)
	{
		const auto player = conn->getPlayer();

		if (player == nullptr)
			return;

		const auto squarePlayer = player->getSquarePlayer();

		if (squarePlayer == nullptr)
			return;

		conn->send(SetPosition(squarePlayer));
		squarePlayer->setState(0);
	}
};