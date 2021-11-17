#pragma once

#include <array>
#include <cstdint>

#include "packets/LobbyServerPacket.h"

class SendUpdateWeapons : public LobbyServerPacket
{
public:
	SendUpdateWeapons(const uint16_t characterOffset, const std::array<uint64_t, 4>& weapons) : LobbyServerPacket(624)
	{
		writeShort(characterOffset);

		for (const auto& weapon : weapons)
		{
			writeLong(weapon);
			writeLong(0);
		}
	}
};