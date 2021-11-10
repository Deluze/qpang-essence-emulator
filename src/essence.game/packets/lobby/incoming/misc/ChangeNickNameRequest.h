#pragma once

#include "ChangeNickNameFailResponse.h"
#include "core/communication/packet/PacketEvent.h"

class ChangeNickNameRequest final : public PacketEvent
{
public:
	enum LS_CHANGE_NICKNAME_FAIL
	{
		FR_LS_DUPLICATE_NICKNAME = 820,
		FR_LS_INVALID_NICKNAME = 827,
	};

	void handle(const QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		const auto player = conn->getPlayer();

		// TODO: Find out what this packet does.

		// TODO: Check if player has nickname changer in inventory.
		// TODO: Change player nickname.
		// TODO: Remove nickname changer from player inventory.
	}
};
