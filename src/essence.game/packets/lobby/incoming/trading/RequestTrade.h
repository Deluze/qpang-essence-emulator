#pragma once

#include "core/communication/packet/PacketEvent.h"

class RequestTrade final : public PacketEvent
{
public:
	// Trade cancel 884
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto player = conn->getPlayer();

		const auto targetPlayerId = packet.readInt();
		const auto targetPlayer = Game::instance()->getPlayer(targetPlayerId);

		if (targetPlayer == nullptr)
		{
			// TODO: Send trade fail?
			// 877 - Trade error "The trading process has stopped because of error caused in the system internally"?
			conn->send(LobbyServerPacket(877));

			return;
		}

		std::cout << "RequestTrade::handle >> Player " << player->getId() << " has requested to trade with " << targetPlayerId << std::endl;

		// 876 - Shows trade request menu.
		//conn->send(LobbyServerPacket(876));

		conn->send(LobbyServerPacket(877));

		//targetPlayer->send(LobbyServerPacket(881)); // The other side has cancelled the request.
	}
};