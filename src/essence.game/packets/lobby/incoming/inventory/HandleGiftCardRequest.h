#pragma once

#include "SendGiftInventoryCardFail.h"
#include "core/communication/packet/PacketEvent.h"

#include "qpang/Game.h"
#include "qpang/player/Player.h"

class HandleGiftCardRequest final : public PacketEvent
{
public:
	//814 = LS_SEND_PRESENT_INDEC_FAIL
	enum LS_SEND_PRESENT_INDEC_FAIL
	{
		INVENTORY_FULL_UNSEAL = 362,
		SEND_GIFT_FAIL_TARGET_NOT_EXIST = 365,
		GIFT_FAIL_SEND_TO_SELF = 846,
		GIFT_FAIL_UNTRADABLE_CARD = 871
	};

	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto player = conn->getPlayer();

		const auto nickname = packet.readStringTerminated(16);
		const auto cardId = packet.readLong();
		// ReSharper disable once CppDeclaratorNeverUsed
		const auto idk = packet.readInt();

		auto card = player->getInventoryManager()->get(cardId);

		if (!card.isGiftable)
		{
			conn->send(SendGiftInventoryCardFail(GIFT_FAIL_UNTRADABLE_CARD));

			return;
		}

		const auto targetPlayer = Game::instance()->getPlayer(nickname);

		if (targetPlayer == nullptr)
		{
			conn->send(SendGiftInventoryCardFail(SEND_GIFT_FAIL_TARGET_NOT_EXIST));

			return;
		}

		if (player->getId() == targetPlayer->getId())
		{
			conn->send(SendGiftInventoryCardFail(GIFT_FAIL_SEND_TO_SELF));

			return;
		}

		if (!targetPlayer->getInventoryManager()->hasSpace() /*|| !targetPlayer->getInventoryManager()->hasGiftSpace()*/)
		{
			conn->send(SendGiftInventoryCardFail(INVENTORY_FULL_UNSEAL));

			return;
		}

		if (player->getEquipmentManager()->hasEquipped(cardId))
		{
			conn->send(SendGiftInventoryCardFail(GIFT_FAIL_UNTRADABLE_CARD));

			return;
		}

		player->getInventoryManager()->giftCard(card, targetPlayer);
	}
};