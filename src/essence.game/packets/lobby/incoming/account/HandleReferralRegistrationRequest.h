#pragma once

#include "core/communication/packet/PacketEvent.h"
#include <packets/lobby/outgoing/account/SendReferralRegistrationSuccess.h>

class HandleReferralRegistrationRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto player = conn->getPlayer();

		const auto referralNickname = packet.readStringTerminated(16);

		// TODO: Do stuff with referral nickname.

		conn->send(SendReferralRegistrationSuccess());
	}
};
