#pragma once

#include "packets/LobbyServerPacket.h"

class SendReferralRegistrationError : public LobbyServerPacket
{
public:
	explicit SendReferralRegistrationError() : LobbyServerPacket(678)
	{
	}
};
