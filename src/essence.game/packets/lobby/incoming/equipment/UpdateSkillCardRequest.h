#pragma once

#include "UpdateSkillCardResponse.h"
#include "core/communication/packet/PacketEvent.h"

class UpdateSkillCardRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto player = conn->getPlayer();

		//for (int i = 0; i < 5; i++)
		//{
		//	for (int j = 0; j < 5; j++)
		//	{
		//		const auto cardId = packet.readLong();

		//		std::cout << "CardId: " << cardId << std::endl;

		//		packet.readEmpty(35);
		//	}
		//}

		const auto currentlyEquippedSkillCards = player->getEquipmentManager()->getEquippedSkillCards();

		for (const auto& equippedSkillCard : currentlyEquippedSkillCards)
		{
			player->getInventoryManager()->setCardActive(equippedSkillCard.id, false);
		}

		const auto firstCardId = packet.readLong();

		packet.readEmpty(35);

		const auto secondCardId = packet.readLong();

		packet.readEmpty(35);

		const auto thirdCardId = packet.readLong();

		std::cout << "Card ids: " << firstCardId << ", " << secondCardId << ", " << thirdCardId << std::endl;

		player->getEquipmentManager()->setSkillCards({ firstCardId, secondCardId, thirdCardId });

		for (const auto equippedSkillCard : player->getEquipmentManager()->getEquippedSkillCards())
		{
			std::cout << "Equipped skillcard card id: " << equippedSkillCard.id << std::endl;

			player->getInventoryManager()->setCardActive(equippedSkillCard.id, true);
		}

		conn->send(UpdateSkillCardResponse(player->getEquipmentManager()->getEquippedSkillCards()));
	}
};