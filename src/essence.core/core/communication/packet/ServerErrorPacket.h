#pragma once

#include "core/communication/packet/ServerPacket.h"

class ServerErrorPacket : public ServerPacket
{
protected:
	ServerErrorPacket(uint16_t packetId) : ServerPacket(packetId)
	{
		//writeInt(0); // ?
	}
};