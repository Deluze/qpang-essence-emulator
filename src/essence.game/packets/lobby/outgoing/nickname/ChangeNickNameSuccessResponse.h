#pragma once

#include "packets/LobbyServerPacket.h"

class ChangeNickNameSuccessResponse : public LobbyServerPacket
{
public:
	explicit ChangeNickNameSuccessResponse(const std::u16string nickName) : LobbyServerPacket(838)
	{
		writeString(nickName, 16);
	}
};