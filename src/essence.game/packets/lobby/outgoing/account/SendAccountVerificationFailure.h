#pragma once

#include "packets/LobbyServerPacket.h"

class SendAccountVerificationFailure : public LobbyServerPacket
{
public:
	SendAccountVerificationFailure() : LobbyServerPacket(602)
	{
		writeInt(204);
	}
};