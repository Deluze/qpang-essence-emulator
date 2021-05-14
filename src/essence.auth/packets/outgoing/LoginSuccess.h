#pragma once

#include <array>
#include <cstdint>

#include "core/communication/packet/ServerPacket.h"

class LoginSuccess : public ServerPacket
{
public:
	LoginSuccess(const std::array<uint8_t, 16>& uuid, uint32_t gameHost) : ServerPacket(501)
	{
		writeInt(gameHost);
		writeArray<uint8_t, 16>(uuid);
	}
};