#pragma once

#include "packets/LobbyServerPacket.h"

class SendValidateNickNameSuccess : public LobbyServerPacket
{
public:
	explicit SendValidateNickNameSuccess() : LobbyServerPacket(668) // 668 = LS_CHECK_NICKNAME_SUCCESS, 669 = LS_CHECK_NICKNAME_FAIL
	{
	}
};