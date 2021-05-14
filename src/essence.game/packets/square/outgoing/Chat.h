#pragma once

#include <string>

#include "packets/SquareServerPacket.h"

class Chat : public SquareServerPacket
{
public:
	Chat(const std::u16string& sender, const std::u16string& message) : SquareServerPacket(6529)
	{
		writeString(sender, 16);
		writeShort(message.size() % 254);
		writeString(message, message.size() % 254);
	}
};