#pragma once

#include "packets/LobbyServerPacket.h"
#include "qpang/player/friend/Friend.h"

#include "packets/writers/FriendWriter.h"

class SendAcceptIncomingFriend : public LobbyServerPacket
{
public:
	explicit SendAcceptIncomingFriend(const Friend& fr) : LobbyServerPacket(702)
	{
		FriendWriter::write(this, fr);
	}
};