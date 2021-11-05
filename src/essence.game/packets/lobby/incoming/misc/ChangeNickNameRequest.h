#pragma once

#include "core/communication/packet/PacketEvent.h"

class ChangeNickNameRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		const auto player = conn->getPlayer();

		// TODO: Find out what this packet does.
	}
};
