#pragma once

#include "CheckNickNameSuccessResponse.h"
#include "core/communication/packet/PacketEvent.h"

class CheckNickNameRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		const auto player = conn->getPlayer();

		const auto nickname = pack.readStringTerminated(20);

		std::cout << "CheckNickNameRequest::handle >> Name: " << std::string(nickname.begin(), nickname.end()).c_str() << std::endl;

		// TODO: Check if the name is already taken or not.
		player->send(CheckNickNameSuccessResponse());
	}
};
