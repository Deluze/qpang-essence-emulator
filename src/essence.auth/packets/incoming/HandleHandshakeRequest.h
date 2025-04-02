#pragma once

#include <array>

#include "core/communication/packet/PacketEvent.h"
#include "packets/outgoing/SendHandshakeResponse.h"

class HandleHandshakeRequest : public PacketEvent
{
public:
	bool isUnauthorizedPacket() override
	{
		return true;
	}

	void handle(QpangConnection::Ptr conn, QpangPacket& packet)
	{
		std::array<char, 4> keyPart = conn->getKeyPart();
		conn->send(SendHandshakeResponse(keyPart));
	}
};