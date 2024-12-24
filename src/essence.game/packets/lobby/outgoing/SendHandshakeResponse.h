#pragma once

#include <array>

#include "packets/LobbyServerPacket.h"

class SendHandshakeResponse : public LobbyServerPacket
{
public:
	explicit SendHandshakeResponse(const std::array<char, 4>& key) : LobbyServerPacket(2)
	{
		setCryptoType(BlowfishInstance::CryptoType::PUBLIC);

		writeArray<char, 4>(key);
		writeInt(0);
	}
};