#pragma once

#include <vector>

#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"
#include "qpang/square/SquareManager.h"
#include "qpang/square/SquarePlayer.h"

#include "packets/square/outgoing/Players.h"

class ChatRequest : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet)
	{
		packet.readEmpty(34);
		const auto length = packet.readShort();
		const auto message = packet.readString(length % 254);

		const auto player = conn->getPlayer();
		if (player == nullptr)
			return;

		const auto squarePlayer = player->getSquarePlayer();
		if (squarePlayer == nullptr)
			return;

		const auto actualMessage = Game::instance()->getChatManager()->chat(player, message);

		if (player->isMuted() && !actualMessage.empty())
			return player->broadcast(u"You are currently muted. Your message has not been sent.");

		if (actualMessage.empty())
			return;

		squarePlayer->chat(actualMessage);
	}
};