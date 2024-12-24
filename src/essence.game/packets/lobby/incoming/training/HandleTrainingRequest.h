#pragma once

#include "core/communication/packet/PacketEvent.h"

class HandleTrainingRequest final : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& pack) override
	{
	
	}
};