#pragma once

#include <array>
#include <cstdint>

#include "core/communication/packet/ServerPacket.h"

class SendAccountLoginSuccess : public ServerPacket
{
public:
	SendAccountLoginSuccess(const std::array<uint8_t, 16>& uuid, uint32_t gameHost) : ServerPacket(501)
	{
		writeInt(gameHost);
		writeArray<uint8_t, 16>(uuid);
	}
};