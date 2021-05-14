#pragma once

#include <vector>

#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"
#include "qpang/square/SquareManager.h"
#include "qpang/square/SquarePlayer.h"

#include "packets/square/outgoing/Players.h"

class UpdatePosition : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet)
	{
		const auto moveType = packet.readByte();
		const auto direction = packet.readByte();
		const auto position = packet.readArray<float, 3>();

		const auto player = conn->getPlayer();

		if (player == nullptr)
			return;

		auto squarePlayer = player->getSquarePlayer();

		if (squarePlayer == nullptr)
			return;

		squarePlayer->move(position, direction, moveType);
	}
};