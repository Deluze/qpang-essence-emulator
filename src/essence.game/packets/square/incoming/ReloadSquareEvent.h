#pragma once

#include "core/communication/packet/PacketEvent.h"
#include "packets/square/outgoing/SetPosition.h"
#include "qpang/player/Player.h"
#include "qpang/square/SquarePlayer.h"

class ReloadSquareEvent final : public PacketEvent
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

		conn->send(SetPosition(squarePlayer));

		squarePlayer->setState(0);
	}
};
