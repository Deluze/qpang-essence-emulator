#pragma once

#include "packets/LobbyServerPacket.h"

class SendAccountDuplicateLogin : public LobbyServerPacket
{
public:
	SendAccountDuplicateLogin() : LobbyServerPacket(602)
	{
		writeInt(802);
	}
};