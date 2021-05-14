#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/player/Player.h"
#include "qpang/square/Square.h"
#include "qpang/square/SquarePlayer.h"

class EmoteEvent : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet)
	{
		const auto emoteId = packet.readInt();

		const auto player = conn->getPlayer();
		if (player == nullptr)
			return;

		const auto squarePlayer = player->getSquarePlayer();
		if (squarePlayer == nullptr)
			return;

		squarePlayer->emote(emoteId);
	}
};