#pragma once

#include "packets/LobbyServerPacket.h"

#include "qpang/player/friend/Friend.h"

class FriendRemoved : public LobbyServerPacket
{
public:
	FriendRemoved(const Friend& fr) : LobbyServerPacket(716)
	{
		writeInt(fr.playerId);
		writeEmpty(8);
		writeString(fr.nickname, 16);
	}
};