#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"
#include "qpang/player/Player.h"
#include "qpang/square/SquareManager.h"

class JoinSquare : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		auto playerId = packet.readInt();
		auto squareId = packet.readInt();

		auto player = conn->getPlayer();

		if (player == nullptr)
			return;

		const auto squarePlayer = player->getSquarePlayer();
		const auto isInSquare = squarePlayer != nullptr;
		
		if (isInSquare)
		{
			squarePlayer->getSquare()->remove(player->getId());
			player->leaveSquare();

			auto square = Game::instance()->getSquareManager()->get(squareId);

			if (square == nullptr)
				return;

			square->add(player);
		}
		else
		{
			auto square = Game::instance()->getSquareManager()->getAvailableSquare();
			square->add(player);
		}
	}
};