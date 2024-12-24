#pragma once

#include "core/communication/packet/PacketEvent.h"
#include "packets/lobby/outgoing/player/SendUpdatePlayerRanking.h"

class HandleGetPlayerRankingRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto playerId = conn->getPlayer()->getId();
		const auto leaderboardPosition = Game::instance()->getLeaderboard()->getPosition(playerId);

		conn->send(SendUpdatePlayerRanking(leaderboardPosition));
	}
};
