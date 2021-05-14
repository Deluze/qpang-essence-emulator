#pragma once

#include <array>

#include "packets/LobbyServerPacket.h"
#include "packets/writers/InventoryCardWriter.h"
#include "qpang/player/inventory/InventoryCard.h"

class EquippedSkillCards : public LobbyServerPacket
{
public:
	EquippedSkillCards(const std::array<InventoryCard, 3>& skillCards) : LobbyServerPacket(647)
	{
		for (size_t i = 0; i < 5; i++)
			for (const InventoryCard& card : skillCards)
				InventoryCardWriter::write(this, card);
	}
};