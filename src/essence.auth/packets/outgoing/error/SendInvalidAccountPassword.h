#pragma once

#include "core/communication/packet/ServerErrorPacket.h"

class SendInvalidAccountPassword : public ServerErrorPacket
{
public:
	SendInvalidAccountPassword() : ServerErrorPacket(502)
	{
		writeInt(1101);
	}
};