#pragma once

#include "core/communication/packet/PacketEvent.h"

class TrainingRequest final : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& pack) override
	{
	
	}
};