#pragma once

#include "SendInventoryCards.h"
#include "SendUpdateSkillSet.h"
#include "core/communication/packet/PacketEvent.h"

auto constexpr CARD_DATA_LENGTH = 43;
auto constexpr SKILL_CARDS_PER_SET = 3;

class UpdateSkillCardSetRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto player = conn->getPlayer();

		std::vector<uint64_t> incomingCardIds{};

		for (int i = 0; i < SKILL_CARDS_PER_SET; i++)
		{
			const auto incomingCardId = packet.readLong();

			packet.readEmpty(CARD_DATA_LENGTH - 8);

			if (!player->getInventoryManager()->hasCard(incomingCardId))
			{
				continue;
			}

			const auto incomingCard = player->getInventoryManager()->get(incomingCardId);

			bool hasCardId = false;

			for (const auto cardId : incomingCardIds)
			{
				if (const auto card = player->getInventoryManager()->get(cardId); (card.itemId == incomingCard.itemId))
				{
					hasCardId = true;

					break;
				}
			}

			if (!hasCardId)
			{
				incomingCardIds.push_back(incomingCardId);
			}
		}

		toggleActiveStateForInventoryCards(player, false);

		player->getEquipmentManager()->setSkillCardIds(incomingCardIds);

		toggleActiveStateForInventoryCards(player, true);

		conn->send(SendUpdateSkillSet(player->getEquipmentManager()->getEquippedSkillCards()));
	}
private:
	static void toggleActiveStateForInventoryCards(const Player::Ptr& player, const bool isActive)
	{
		for (const auto& equippedSkillCard : player->getEquipmentManager()->getEquippedSkillCards())
		{
			player->getInventoryManager()->setCardActive(equippedSkillCard.id, isActive);
		}
	}
};