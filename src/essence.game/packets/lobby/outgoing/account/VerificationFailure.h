#pragma once

#include "packets/LobbyServerPacket.h"

class VerificationFailure : public LobbyServerPacket
{
public:
	VerificationFailure() : LobbyServerPacket(602)
	{
		writeInt(204);
	}
};