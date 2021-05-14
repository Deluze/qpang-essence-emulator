#pragma once

#include "core/communication/packet/ServerErrorPacket.h"

class InvalidUsername : public ServerErrorPacket
{
public:
	InvalidUsername() : ServerErrorPacket(502)
	{
		writeInt(1102);
	}
};