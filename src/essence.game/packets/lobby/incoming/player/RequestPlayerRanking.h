#pragma once

#include "core/communication/packet/PacketEvent.h"

#include "packets/lobby/outgoing/player/UpdatePlayerRanking.h"

class RequestPlayerRanking : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet)
	{
		const auto playerId = conn->getPlayer()->getId();
		const auto leaderboardPosition = Game::instance()->getLeaderboard()->getPosition(playerId);

		conn->send(UpdatePlayerRanking(leaderboardPosition));
	}
};