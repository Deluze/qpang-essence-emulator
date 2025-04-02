#pragma once

#include "core/communication/PacketHandler.h"

class SquarePacketHandler final : public PacketHandler
{
public:
	SquarePacketHandler();
	void handle(std::shared_ptr<QpangConnection> conn, QpangPacket packet) override;
};