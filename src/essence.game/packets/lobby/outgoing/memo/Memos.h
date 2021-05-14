#pragma once

#include <vector>

#include "packets/LobbyServerPacket.h"
#include "qpang/player/memo/Memo.h"

class Memos : public LobbyServerPacket
{
public:
	Memos(const std::vector<Memo>& memos) : LobbyServerPacket(726)
	{
		auto size = static_cast<uint16_t>(memos.size());

		writeShort(size);
		writeShort(size);
		writeShort(size);

		for (const auto& memo : memos)
		{
			writeLong(memo.id);
			writeInt(memo.senderId);
			writeInt(memo.created);
			writeString(memo.nickname, 16);
			writeString(memo.message, 100);
			writeFlag(memo.isOpened);
		}
	}
};