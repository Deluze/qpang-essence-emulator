#pragma once

#include "ValidateNickNameFailResponse.h"
#include "ValidateNickNameSuccessResponse.h"
#include "core/communication/packet/PacketEvent.h"

class ValidateNickNameRequest final : public PacketEvent
{
public:
	enum LS_CHECK_NICKNAME_FAIL
	{
		FR_LS_DUPLICATE_NICKNAME = 0x334,	// 820
		FR_LS_INVALID_NICKNAME = 0x335		// 821
	};

	void handle(const QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		const auto player = conn->getPlayer();

		const auto nickName = pack.readStringTerminated(20);

		if (nickName.length() < 4 || nickName.length() > 16)
		{
			conn->send(ValidateNickNameFailResponse(FR_LS_INVALID_NICKNAME));

			return;
		}

		// TODO: Check if nickname has special characters in name (regex).
		// Nickname may only contain certain characters.

		const auto statement = DATABASE->prepare("SELECT * FROM `players` WHERE name = ? LIMIT 1");

		statement->bindString(nickName);

		if (const auto result = statement->fetch(); result->hasResults())
		{
			conn->send(ValidateNickNameFailResponse(FR_LS_DUPLICATE_NICKNAME));

			return;
		}

		conn->send(ValidateNickNameSuccessResponse());
	}
};
