#pragma once

#include "packets/LobbyServerPacket.h"
#include "qpang/leaderboard/Leaderboard.h"

class UpdatePlayerRanking : public LobbyServerPacket
{
public:
	explicit UpdatePlayerRanking(const Leaderboard::Position& position) : LobbyServerPacket(792)
	{
		writeEmpty(4);
		writeInt(position.rank);
		writeInt(position.difference);
	}
};
