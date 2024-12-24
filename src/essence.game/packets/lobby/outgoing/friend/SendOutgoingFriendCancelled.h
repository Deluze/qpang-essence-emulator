	#pragma once

	#include "packets/LobbyServerPacket.h"
	#include "qpang/player/Player.h"

	class SendOutgoingFriendCancelled : public LobbyServerPacket
	{
	public:
		explicit SendOutgoingFriendCancelled(const Player::Ptr player) : LobbyServerPacket(708)
		{
			writeInt(player->getId());
			writeEmpty(8);
			writeString(player->getName(), 16);
		}
	};