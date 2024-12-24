#pragma once

#include "packets/LobbyServerPacket.h"
#include "qpang/player/friend/Friend.h"

class SendFriendRemoved : public LobbyServerPacket
{
public:
	explicit SendFriendRemoved(const Friend& fr) : LobbyServerPacket(716)
	{
		writeInt(fr.playerId);
		writeEmpty(8);
		writeString(fr.nickname, 16);
	}
};
