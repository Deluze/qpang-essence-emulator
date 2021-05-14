#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/player/Player.h"
#include "qpang/square/Square.h"

class UpdateStateEvent : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto playerId = packet.readInt();
		const auto state = packet.readInt();
		const auto roomId = packet.readByte();

		const auto player = conn->getPlayer();

		if (player == nullptr)
			return;

		const auto squarePlayer = player->getSquarePlayer();

		if (squarePlayer == nullptr)
			return;

		if (state == 7)
			squarePlayer->setState(7);
		else if (state == 5)
			squarePlayer->setState(5, roomId);
	}
};