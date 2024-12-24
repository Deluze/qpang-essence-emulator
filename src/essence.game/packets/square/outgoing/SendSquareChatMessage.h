#pragma once

#include <string>

#include "packets/SquareServerPacket.h"

class SendSquareChatMessage : public SquareServerPacket
{
public:
	SendSquareChatMessage(const std::u16string& sender, const std::u16string& message) : SquareServerPacket(6529)
	{
		writeString(sender, 16);
		writeShort(message.size() % 254);
		writeString(message, message.size() % 254);
	}
};