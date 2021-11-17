#pragma once

#include "packets/SquareServerPacket.h"

class SendDeleteSquareEntry : public SquareServerPacket
{
public:
	explicit SendDeleteSquareEntry(const uint32_t id) : SquareServerPacket(6543)
	{
		writeInt(1); // 0 = update, 1 = delete
		writeEmpty(5);
		writeInt(id);
	}
};