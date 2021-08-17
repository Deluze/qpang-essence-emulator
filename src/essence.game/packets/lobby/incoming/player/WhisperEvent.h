#pragma once

#include "core/communication/packet/PacketEvent.h"
#include "packets/lobby/outgoing/player/SendWhisper.h"
#include "qpang/Game.h"
#include "qpang/player/Player.h"

class WhisperEvent final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		pack.readEmpty(4);

		const auto length = pack.readShort() % 254;

		const auto nickname = pack.readStringTerminated(16);
		const auto message = pack.readStringTerminated(length);

		if (const auto player = Game::instance()->getOnlinePlayer(nickname); player != nullptr)
		{
			if (const auto ourPlayer = conn->getPlayer(); ourPlayer != nullptr && ourPlayer->isMuted())
			{
				return ourPlayer->broadcast(u"You are currently restricted from using the chat.");
			}

			player->whisper(conn->getPlayer()->getName(), message);

			conn->send(SendWhisper(player->getName(), message));
		}
	}
};
