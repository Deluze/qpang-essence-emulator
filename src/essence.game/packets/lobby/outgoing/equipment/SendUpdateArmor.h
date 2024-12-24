#pragma once

#include <array>
#include <cstdint>

#include "packets/LobbyServerPacket.h"

class SendUpdateArmor : public LobbyServerPacket
{
public:
	SendUpdateArmor(const uint16_t characterOffset, const std::array<uint64_t, 9>& armor) : LobbyServerPacket(621)
	{
		writeShort(characterOffset);

		for (const auto& armorPiece : armor)
		{
			writeLong(armorPiece);
		}
	}
};