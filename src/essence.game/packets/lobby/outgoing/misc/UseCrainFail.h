#pragma once

#include "packets/LobbyServerPacket.h"

class UseCrainFail: public LobbyServerPacket
{
public:
	UseCrainFail(uint16_t character) : LobbyServerPacket(899)
	{
		writeShort(character);
	}
};