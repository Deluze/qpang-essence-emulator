#pragma once

#include "packets/LobbyServerPacket.h"
#include "qpang/player/Player.h"

class IncomingFriendCancelled : public LobbyServerPacket
{
public:
	explicit IncomingFriendCancelled(const Player::Ptr player) : LobbyServerPacket(712)
	{
		writeInt(player->getId());
		writeEmpty(8);
		writeString(player->getName(), 16);
	}
};