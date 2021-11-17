#pragma once

#include <cstdint>

class SendUseEmote : public SquareServerPacket
{
public:
	SendUseEmote(const uint32_t playerId, const uint32_t emoteId) : SquareServerPacket(6558)
	{
		writeInt(playerId);
		writeInt(emoteId);
	}
};