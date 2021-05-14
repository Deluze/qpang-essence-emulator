#pragma once

#include <array>

#include "core/communication/packet/PacketEvent.h"
#include "packets/lobby/outgoing/HandshakeResponse.h"

class Handshake : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& packet)
	{
		std::array<char, 4> keyPart = conn->getKeyPart();

		conn->send(HandshakeResponse(keyPart));
	}
};