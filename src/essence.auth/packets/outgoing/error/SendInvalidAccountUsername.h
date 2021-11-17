#pragma once

#include "core/communication/packet/ServerErrorPacket.h"

class SendInvalidAccountUsername : public ServerErrorPacket
{
public:
	SendInvalidAccountUsername() : ServerErrorPacket(502)
	{
		writeInt(1102);
	}
};