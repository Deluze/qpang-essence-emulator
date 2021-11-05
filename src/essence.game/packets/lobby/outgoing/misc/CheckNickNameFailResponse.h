#pragma once

#include "packets/LobbyServerPacket.h"

class CheckNickNameFailResponse : public LobbyServerPacket
{
public:
	explicit CheckNickNameFailResponse() : LobbyServerPacket(669) // 668 = LS_CHECK_NICKNAME_SUCCESS, 669 = LS_CHECK_NICKNAME_FAIL
	{
	}
};