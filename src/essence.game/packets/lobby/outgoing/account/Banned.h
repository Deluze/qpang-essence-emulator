#pragma once

#include "packets/LobbyServerPacket.h"

class Banned : public LobbyServerPacket
{
public:
	Banned() : LobbyServerPacket(602)
	{
		writeInt(819);
	}
};