#pragma once

#include "packets/LobbyServerPacket.h"

class SendChangeNickNameSuccess : public LobbyServerPacket
{
public:
	explicit SendChangeNickNameSuccess(const std::u16string nickName) : LobbyServerPacket(838)
	{
		writeString(nickName, 16);
	}
};