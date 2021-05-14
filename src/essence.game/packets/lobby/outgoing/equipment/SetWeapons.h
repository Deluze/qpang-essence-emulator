#pragma once

#include <cstdint>
#include <array>

#include "packets/LobbyServerPacket.h"

class SetWeapons : public LobbyServerPacket
{
public:
	SetWeapons(const uint16_t characterOffset, const std::array<uint64_t, 4>& weapons) : LobbyServerPacket(624)
	{
		writeShort(characterOffset);

		for (const auto& weapon : weapons)
		{
			writeLong(weapon);
			writeLong(0);
		}
	}
};