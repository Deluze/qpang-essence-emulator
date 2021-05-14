#pragma once

#include "packets/LobbyServerPacket.h"
#include "packets/writers/FriendWriter.h"
#include "qpang/player/friend/Friend.h"

class AddIncomingFriend : public LobbyServerPacket
{
public:
	AddIncomingFriend(const Friend& fr) : LobbyServerPacket(700)
	{
		FriendWriter::write(this, fr);
	}
};