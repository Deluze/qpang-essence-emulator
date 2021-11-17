#pragma once

#include "packets/LobbyServerPacket.h"
#include "qpang/player/Player.h"

class SendIncomingFriendCancelled : public LobbyServerPacket
{
public:
	explicit SendIncomingFriendCancelled(const Player::Ptr player) : LobbyServerPacket(712)
	{
		writeInt(player->getId());
		writeEmpty(8);
		writeString(player->getName(), 16);
	}
};