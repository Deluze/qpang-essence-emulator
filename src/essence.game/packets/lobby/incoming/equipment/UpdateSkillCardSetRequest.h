// ReSharper disable CppUseStructuredBinding
// ReSharper disable CppTooWideScopeInitStatement
#pragma once

#include "Inventory.h"
#include "UpdateSkillCardSetResponse.h"
#include "core/communication/packet/PacketEvent.h"

auto constexpr CARD_DATA_LENGTH = 43;
auto constexpr SKILL_CARDS_PER_SET = 3;

class UpdateSkillCardSetRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& packet) override
	{
		const auto player = conn->getPlayer();

		const auto readCardIds = readAndGetCardIds(packet, player);

		player->getEquipmentManager()->setSkillCards(readCardIds);

		conn->send(UpdateSkillCardSetResponse(player->getEquipmentManager()->getEquippedSkillCards()));
	}
private:
	static void toggleActiveStateForInventoryCards(const Player::Ptr& player, const bool isActive)
	{
		for (const auto& equippedSkillCard : player->getEquipmentManager()->getEquippedSkillCards())
		{
			player->getInventoryManager()->setCardActive(equippedSkillCard.id, isActive);
		}
	}

	static std::vector<uint64_t> readAndGetCardIds(QpangPacket& packet, const Player::Ptr& player)
	{
		std::vector<uint64_t> readCardIds = {};

		for (size_t i = 0; i < 3; i++)
		{
			if (const auto readCardId = packet.readLong(); player->getInventoryManager()->hasCard(readCardId))
			{
				// Player has card, now lets check player has not equipped multiple cards with the same itemId.
				const auto inventoryCard = player->getInventoryManager()->get(readCardId);

				auto isDuplicateItemId = false;

				for (const auto cardId : readCardIds)
				{
					const auto lookupInventoryCard = player->getInventoryManager()->get(cardId);

					if (inventoryCard.itemId == lookupInventoryCard.itemId)
					{
						isDuplicateItemId = true;
					}
				}

				readCardIds.push_back(isDuplicateItemId ? 0 : readCardId);
			}
			else
			{
				readCardIds.push_back(0);
			}

			packet.readEmpty(35);
		}

		return readCardIds;
	}
};