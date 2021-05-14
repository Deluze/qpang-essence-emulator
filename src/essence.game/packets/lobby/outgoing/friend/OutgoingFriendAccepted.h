#pragma once

#include "packets/LobbyServerPacket.h"
#include "qpang/player/friend/Friend.h"

#include "packets/writers/FriendWriter.h"

class OutgoingFriendAccepted : public LobbyServerPacket
{
public:
	OutgoingFriendAccepted(const Friend& fr) : LobbyServerPacket(704)
	{
		FriendWriter::write(this, fr);
	}
};