#pragma once

#include "packets/LobbyServerPacket.h"

class SendAccountRegistrationSuccess : public LobbyServerPacket
{
public:
	explicit SendAccountRegistrationSuccess(const std::u16string nickName, const uint16_t firstCharacterId, const uint16_t secondCharacterId) : LobbyServerPacket(671)
	{
		writeString(nickName, 16);
		writeShort(firstCharacterId);
		writeEmpty(4);
		writeShort(secondCharacterId);
	}
};
