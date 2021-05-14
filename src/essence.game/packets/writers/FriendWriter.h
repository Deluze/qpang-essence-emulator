#pragma once

#include "core/communication/packet/ServerPacket.h"

#include "qpang/player/friend/Friend.h"

class FriendWriter
{
public:
	static void write(ServerPacket* packet, const Friend& fr)
	{
		packet->writeInt(fr.playerId);
		packet->writeEmpty(4);
		packet->writeByte(fr.state);
		packet->writeFlag(fr.isOnline);
		packet->writeShort(fr.level);
		packet->writeString(fr.nickname, 16);
	}
};