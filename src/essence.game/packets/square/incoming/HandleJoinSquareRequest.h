#pragma once

#include "core/communication/packet/PacketEvent.h"
#include "qpang/Game.h"
#include "qpang/player/Player.h"
#include "qpang/square/SquareManager.h"

class HandleJoinSquareRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		auto playerId = packet.readInt();
		const auto squareId = packet.readInt();

		const auto player = conn->getPlayer();

		if (player == nullptr)
		{
			return;
		}

		if (const auto squarePlayer = player->getSquarePlayer(); squarePlayer != nullptr)
		{
			squarePlayer->getSquare()->remove(player->getId());
			player->leaveSquare();

			const auto square = Game::instance()->getSquareManager()->get(squareId);

			if (square == nullptr)
			{
				return;
			}

			square->add(player);
		}
		else
		{
			const auto square = Game::instance()->getSquareManager()->getAvailableSquare();

			square->add(player);
		}
	}
};
