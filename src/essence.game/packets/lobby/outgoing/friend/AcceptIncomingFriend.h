#pragma once

#include "packets/LobbyServerPacket.h"
#include "qpang/player/friend/Friend.h"

#include "packets/writers/FriendWriter.h"

class AcceptIncomingFriend : public LobbyServerPacket
{
public:
	explicit AcceptIncomingFriend(const Friend& fr) : LobbyServerPacket(702)
	{
		FriendWriter::write(this, fr);
	}
};