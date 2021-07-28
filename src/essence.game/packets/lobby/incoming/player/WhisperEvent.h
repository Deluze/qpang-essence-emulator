#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"
#include "qpang/player/Player.h"

#include "packets/lobby/outgoing/player/SendWhisper.h"

class WhisperEvent : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		pack.readEmpty(4);

		auto length = pack.readShort() % 254;
		auto nickname = pack.readStringTerminated(16);
		auto message = pack.readStringTerminated(length);

		if (auto player = Game::instance()->getOnlinePlayer(nickname); player != nullptr)
		{
			const auto ourPlayer = conn->getPlayer();

			if (ourPlayer != nullptr && ourPlayer->isMuted())
				return ourPlayer->broadcast(u"You are currently restricted from using the chat.");

			player->whisper(conn->getPlayer()->getName(), message);
			conn->send(SendWhisper(player->getName(), message));
		}
	}
};