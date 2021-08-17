#pragma once

#include "packets/LobbyServerPacket.h"
#include "packets/writers/FriendWriter.h"
#include "qpang/player/friend/Friend.h"

class AddOutgoingFriend : public LobbyServerPacket
{
public:
	explicit AddOutgoingFriend(const Friend& fr) : LobbyServerPacket(698)
	{
		FriendWriter::write(this, fr);
	}
};