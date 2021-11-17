#pragma once

#include "packets/LobbyServerPacket.h"
#include "packets/writers/FriendWriter.h"
#include "qpang/player/friend/Friend.h"

class SendOutgoingFriendAccepted : public LobbyServerPacket
{
public:
	explicit SendOutgoingFriendAccepted(const Friend& fr) : LobbyServerPacket(704)
	{
		FriendWriter::write(this, fr);
	}
};
