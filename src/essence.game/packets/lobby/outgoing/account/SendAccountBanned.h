#pragma once

#include "packets/LobbyServerPacket.h"

class SendAccountBanned : public LobbyServerPacket
{
public:
	SendAccountBanned() : LobbyServerPacket(602)
	{
		writeInt(819);
	}
};