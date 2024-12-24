#pragma once

#include "packets/LobbyServerPacket.h"
#include "packets/writers/FriendWriter.h"
#include "qpang/player/friend/Friend.h"

class SendAddIncomingFriend : public LobbyServerPacket
{
public:
	explicit SendAddIncomingFriend(const Friend& fr) : LobbyServerPacket(700)
	{
		FriendWriter::write(this, fr);
	}
};