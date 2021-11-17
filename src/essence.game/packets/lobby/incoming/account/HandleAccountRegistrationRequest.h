#pragma once

#include "core/communication/packet/PacketEvent.h"

class HandleAccountRegistrationRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto player = conn->getPlayer();

		// TODO: Lookup nickname, and lookup referral nickname and validate them both.

		const auto nickName = packet.readStringTerminated(16);

		// TODO: Remove.
		//player->setName(nickName);

		// Seems like the player nickname gets sent if the referral nickname is empty.
		const auto referralNickName = packet.readStringTerminated(16);

		// TODO: Get the referral code from the database instead of here.

		// TODO: Validate the given character ids.

		const auto firstCharacterId = packet.readShort();
		const auto secondCharacterId = packet.readShort();

		std::cout << "HandleAccountRegistrationRequest::handle >> Nickname: " 
			<< std::string(nickName.cbegin(), nickName.cend()).c_str() << ", Referral Nickname: "
			<< std::string(referralNickName.cbegin(), referralNickName.cend()).c_str() 
			<< ", Chosen Characters: " << firstCharacterId << ", " << secondCharacterId << std::endl;

		// TODO: Find out what the remaining 8 bytes do.

		//conn->send(SendAccountRegistrationSuccess(nickName, firstCharacterId, secondCharacterId));
	}
};
