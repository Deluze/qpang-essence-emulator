#pragma once

#include "core/communication/packet/PacketEvent.h"
#include "qpang/square/SquarePlayer.h"

class HandleUpdateSquarePlayerPositionRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto moveType = packet.readByte();
		const auto direction = packet.readByte();
		const auto position = packet.readArray<float, 3>();

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

		squarePlayer->move(position, direction, moveType);
	}
};
