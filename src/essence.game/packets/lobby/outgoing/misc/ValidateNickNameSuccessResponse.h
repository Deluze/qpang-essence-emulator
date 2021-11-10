#pragma once

#include "packets/LobbyServerPacket.h"

class ValidateNickNameSuccessResponse : public LobbyServerPacket
{
public:
	explicit ValidateNickNameSuccessResponse() : LobbyServerPacket(668) // 668 = LS_CHECK_NICKNAME_SUCCESS, 669 = LS_CHECK_NICKNAME_FAIL
	{
	}
};