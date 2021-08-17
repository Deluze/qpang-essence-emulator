	#pragma once

	#include "packets/LobbyServerPacket.h"
	#include "qpang/player/Player.h"

	class OutgoingFriendCancelled : public LobbyServerPacket
	{
	public:
		explicit OutgoingFriendCancelled(const Player::Ptr player) : LobbyServerPacket(708)
		{
			writeInt(player->getId());
			writeEmpty(8);
			writeString(player->getName(), 16);
		}
	};