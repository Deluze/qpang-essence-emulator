#pragma once

#include "core/communication/packet/ServerErrorPacket.h"

class InvalidPassword : public ServerErrorPacket
{
public:
	InvalidPassword() : ServerErrorPacket(502)
	{
		writeInt(1101);
	}
};