#pragma once

#include "core/communication/packet/ServerErrorPacket.h"

class InvalidVersion : public ServerErrorPacket
{
public:
	InvalidVersion() : ServerErrorPacket(502)
	{
		writeInt(503);
	}
};