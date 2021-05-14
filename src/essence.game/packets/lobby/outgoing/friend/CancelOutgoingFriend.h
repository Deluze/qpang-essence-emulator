#pragma once

#include "packets/LobbyServerPacket.h"
#include "packets/writers/FriendWriter.h"
#include "qpang/player/friend/Friend.h"

class CancelOutgoingFriend : public LobbyServerPacket
{
public:
	CancelOutgoingFriend(const uint32_t playerId) : LobbyServerPacket(710)
	{
		writeInt(playerId);
	}
};