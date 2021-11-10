#pragma once

#include "packets/LobbyServerPacket.h"

class ValidateNickNameFailResponse : public LobbyServerPacket
{
public:
	enum LS_CHECK_NICKNAME_FAIL
	{
		FR_LS_DUPLICATE_NICKNAME = 0x334,	// 820
		FR_LS_INVALID_NICKNAME = 0x335		// 821
	};

	explicit ValidateNickNameFailResponse(const uint32_t code) : LobbyServerPacket(669) // 668 = LS_CHECK_NICKNAME_SUCCESS, 669 = LS_CHECK_NICKNAME_FAIL
	{
		writeInt(code);
	}
};