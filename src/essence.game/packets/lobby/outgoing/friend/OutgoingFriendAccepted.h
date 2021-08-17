#pragma once

#include "packets/LobbyServerPacket.h"
#include "packets/writers/FriendWriter.h"
#include "qpang/player/friend/Friend.h"

class OutgoingFriendAccepted : public LobbyServerPacket
{
public:
	explicit OutgoingFriendAccepted(const Friend& fr) : LobbyServerPacket(704)
	{
		FriendWriter::write(this, fr);
	}
};
