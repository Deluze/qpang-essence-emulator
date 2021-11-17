#pragma once

#include <array>

#include "core/communication/packet/ServerPacket.h"

class SendHandshakeResponse : public ServerPacket
{
public:
	SendHandshakeResponse(std::array<char, 4>& key) : ServerPacket(2)
	{
		setCryptoType(BlowfishInstance::CryptoType::PUBLIC);

		writeArray<char, 4>(key);
		writeInt(0);
	}
};