#pragma once

#include "packets/LobbyServerPacket.h"

class SendUpdatePlayerCharacter : public LobbyServerPacket
{
public:
	explicit SendUpdatePlayerCharacter(const uint16_t character) : LobbyServerPacket(680)
	{
		writeShort(character);
	}
};