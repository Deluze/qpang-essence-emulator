#pragma once

#include "UpdateSkillSetResponse.h"
#include "core/communication/packet/PacketEvent.h"

class UpdateSkillSetRequest final : public PacketEvent
{
public:
	// TODO: Rewrite this so we do not deactivate ALL skillcards but only the one that has changed.
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto player = conn->getPlayer();

		unequipSkillSet(player);

		std::vector<uint64_t> skillCards{};

		for (int i = 0; i < 3; i++)
		{
			if (const auto cardId = packet.readLong(); cardId != 0)
			{
				skillCards.push_back(cardId);
			}

			packet.readEmpty(35);
		}

		player->getEquipmentManager()->setSkillCards(skillCards);

		equipSkillSet(player);

		conn->send(UpdateSkillSetResponse(player->getEquipmentManager()->getEquippedSkillCards()));
	}
private:
	static void unequipSkillSet(const Player::Ptr& player)
	{
		for (const auto& [id, playerOwnerId, itemId, type, periodType, period, isActive, isOpened, isGiftable,
			boostLevel, timeCreated] : player->getEquipmentManager()->getEquippedSkillCards())
		{
			player->getInventoryManager()->setCardActive(id, false);
		}
	}

	static void equipSkillSet(const Player::Ptr& player)
	{
		for (const auto& [id, playerOwnerId, itemId, type, periodType, period, isActive, isOpened, isGiftable,
			boostLevel, timeCreated] : player->getEquipmentManager()->getEquippedSkillCards())
		{
			player->getInventoryManager()->setCardActive(id, true);
		}
	}
};