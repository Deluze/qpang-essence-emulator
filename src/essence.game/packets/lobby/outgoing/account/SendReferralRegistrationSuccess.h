#pragma once

#include "packets/LobbyServerPacket.h"

class SendReferralRegistrationSuccess : public LobbyServerPacket
{
public:
	explicit SendReferralRegistrationSuccess() : LobbyServerPacket(677)
	{
	}
};
