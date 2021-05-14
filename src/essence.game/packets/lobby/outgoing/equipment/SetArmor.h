#pragma once

#include <cstdint>
#include <array>

#include "packets/LobbyServerPacket.h"

class SetArmor : public LobbyServerPacket
{
public:
	SetArmor(const uint16_t characterOffset, const std::array<uint64_t, 9>& armor) : LobbyServerPacket(621)
	{
		writeShort(characterOffset);

		for (const auto& piece : armor)
			writeLong(piece);
	}
};