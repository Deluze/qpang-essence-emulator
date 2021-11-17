#pragma once

#include "packets/LobbyServerPacket.h"
#include "qpang/leaderboard/Leaderboard.h"

class SendUpdatePlayerRanking : public LobbyServerPacket
{
public:
	explicit SendUpdatePlayerRanking(const Leaderboard::Position& position) : LobbyServerPacket(792)
	{
		writeEmpty(4);
		writeInt(position.rank);
		writeInt(position.difference);
	}
};
