#pragma once

#include "packets/LobbyServerPacket.h"

class CheckNickNameSuccessResponse : public LobbyServerPacket
{
public:
	explicit CheckNickNameSuccessResponse() : LobbyServerPacket(668) // 668 = LS_CHECK_NICKNAME_SUCCESS, 669 = LS_CHECK_NICKNAME_FAIL
	{
	}
};