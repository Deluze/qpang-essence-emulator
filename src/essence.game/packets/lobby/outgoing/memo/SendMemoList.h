#pragma once

#include <vector>

#include "packets/LobbyServerPacket.h"
#include "qpang/player/memo/Memo.h"

class SendMemoList : public LobbyServerPacket
{
public:
	explicit SendMemoList(const std::vector<Memo>& memos) : LobbyServerPacket(726)
	{
		const auto size = static_cast<uint16_t>(memos.size());

		writeShort(size);
		writeShort(size);
		writeShort(size);

		// ReSharper disable once CppUseStructuredBinding
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