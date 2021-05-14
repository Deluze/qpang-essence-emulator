#pragma once

#include <vector>

#include "packets/LobbyServerPacket.h"
#include "packets/writers/FriendWriter.h"
#include "qpang/player/friend/Friend.h"

class FriendList : public LobbyServerPacket
{
public:
	FriendList(const std::vector<Friend>& friends) : LobbyServerPacket(695)
	{
		const uint16_t size = static_cast<uint16_t>(friends.size());

		writeShort(size);
		writeShort(size);
		writeShort(size);

		for (const auto& fr : friends)
			FriendWriter::write(this, fr);
	}
};