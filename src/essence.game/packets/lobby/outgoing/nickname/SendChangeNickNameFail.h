#pragma once

#include "packets/LobbyServerPacket.h"

class SendChangeNickNameFail : public LobbyServerPacket
{
public:
	enum LS_CHANGE_NICKNAME_FAIL
	{
		FR_LS_DUPLICATE_NICKNAME = 820,
		FR_LS_INVALID_NICKNAME = 827,
	};

	explicit SendChangeNickNameFail(const uint32_t code) : LobbyServerPacket(839) // LS_CHANGE_NICKNAME_FAIL
	{
		// TODO: Find codes to display the correct message.
		writeInt(code);
	}
};