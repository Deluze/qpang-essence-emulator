#pragma once

#include "packets/LobbyServerPacket.h"
#include "qpang/player/Player.h"

class PlayerInfoInspectorFailed : public LobbyServerPacket
{
public:
	PlayerInfoInspectorFailed(Player::Ptr player) : LobbyServerPacket(693)
	{

	}
};