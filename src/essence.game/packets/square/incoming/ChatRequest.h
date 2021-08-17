#pragma once

#include "core/communication/packet/PacketEvent.h"
#include "qpang/Game.h"
#include "qpang/square/SquarePlayer.h"

class ChatRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		packet.readEmpty(34);

		const auto length = packet.readShort();
		const auto message = packet.readString(length % 254);

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

		const auto actualMessage = Game::instance()->getChatManager()->chat(player, message);

		if (player->isMuted() && !actualMessage.empty()) {
			player->broadcast(u"You are currently restricted from chatting. Your message has not been sent.");

			return;
		}

		if (actualMessage.empty())
		{
			return;
		}

		squarePlayer->chat(actualMessage);
	}
};
