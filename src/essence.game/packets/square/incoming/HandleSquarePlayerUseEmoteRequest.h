#pragma once

#include "core/communication/packet/PacketEvent.h"
#include "qpang/player/Player.h"
#include "qpang/square/SquarePlayer.h"

class HandleSquarePlayerUseEmoteRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto emoteId = packet.readInt();

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

		squarePlayer->emote(emoteId);
	}
};
