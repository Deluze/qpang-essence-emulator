#pragma once

#include <array>

#include "core/communication/packet/PacketEvent.h"
#include "packets/lobby/outgoing/SendHandshakeResponse.h"

class Handshake final : public PacketEvent
{
public:
	bool isUnauthorizedPacket() override
	{
		return true;
	}

	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const std::array<char, 4> keyPart = conn->getKeyPart();

		conn->send(SendHandshakeResponse(keyPart));
	}
};
