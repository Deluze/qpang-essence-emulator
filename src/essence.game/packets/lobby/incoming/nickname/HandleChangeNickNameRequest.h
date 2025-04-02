#pragma once

#include "SendChangeNickNameFail.h"
#include "core/communication/packet/PacketEvent.h"

class HandleChangeNickNameRequest final : public PacketEvent
{
public:
	enum LS_CHANGE_NICKNAME_FAIL
	{
		BAD_REQUEST = 400, // Does not exist in-game.
		FR_LS_DUPLICATE_NICKNAME = 820,
		FR_LS_INVALID_NICKNAME = 827,
	};

	void handle(const QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		const auto player = conn->getPlayer();

		const auto cardId = pack.readLong(); // 8 -> 16
		
		if (!player->getInventoryManager()->hasCard(cardId))
		{
			conn->send(SendChangeNickNameFail(BAD_REQUEST));

			return;
		}

		const auto card = player->getInventoryManager()->get(cardId);

		if (card.period <= 0)
		{
			conn->send(SendChangeNickNameFail(BAD_REQUEST));

			return;
		}

		const auto itemId = pack.readInt(); // 16 -> 20

		if (itemId != NAME_CHANGER)
		{
			conn->send(SendChangeNickNameFail(BAD_REQUEST));

			return;
		}
		
		std::cout << "HandleChangeNickNameRequest::handle >> CardId: " << cardId << ", ItemId: " << itemId << std::endl;

		// TODO: Try to retrieve nickname from player_nickname_registration table.

		// TODO: Change nickname of the player.

		// TODO: Remove card from player inventory.
	}
};
