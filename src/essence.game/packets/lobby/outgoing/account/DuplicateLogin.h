#pragma once

#include "packets/LobbyServerPacket.h"

class DuplicateLogin : public LobbyServerPacket
{
public:
	DuplicateLogin() : LobbyServerPacket(602)
	{
		writeInt(802);
	}
};