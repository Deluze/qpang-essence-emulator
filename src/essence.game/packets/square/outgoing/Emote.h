#pragma once

#include <cstdint>

class Emote : public SquareServerPacket
{
public:
	Emote(const uint32_t playerId, const uint32_t emoteId) : SquareServerPacket(6558)
	{
		writeInt(playerId);
		writeInt(emoteId);
	}
};