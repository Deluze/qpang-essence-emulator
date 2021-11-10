#pragma once

#include "packets/LobbyServerPacket.h"

class ChangeNickNameSuccessResponse : public LobbyServerPacket
{
public:
	explicit ChangeNickNameSuccessResponse() : LobbyServerPacket(838)
	{
	}
};