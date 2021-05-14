#pragma once

#include "packets/LobbyServerPacket.h"

class UpdateCharacter : public LobbyServerPacket
{
public:
	UpdateCharacter(uint16_t character) : LobbyServerPacket(680)
	{
		writeShort(character);
	}
};