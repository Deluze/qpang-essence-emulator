#pragma once

#include <array>

#include "packets/LobbyServerPacket.h"
#include "packets/writers/InventoryCardWriter.h"
#include "qpang/player/inventory/InventoryCard.h"

class EquippedSkillCards : public LobbyServerPacket
{
public:
	explicit EquippedSkillCards(const std::array<InventoryCard, 3>& skillCards) : LobbyServerPacket(647)
	{
		for (size_t i = 0; i < 5; i++)
		{
			for (const InventoryCard& skillCard : skillCards)
			{
				InventoryCardWriter::write(this, skillCard);
			}
		}
	}
};