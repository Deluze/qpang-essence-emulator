#pragma once

#include "packets/LobbyServerPacket.h"
#include "qpang/player/Player.h"

class SendPlayerInfoInspectorError : public LobbyServerPacket
{
public:
	explicit SendPlayerInfoInspectorError(const Player::Ptr& player) : LobbyServerPacket(693)
	{

	}
};