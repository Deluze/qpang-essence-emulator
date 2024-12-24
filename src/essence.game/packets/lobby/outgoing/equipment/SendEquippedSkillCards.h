#pragma once

#include <array>

#include "packets/LobbyServerPacket.h"
#include "packets/writers/InventoryCardWriter.h"
#include "qpang/player/inventory/InventoryCard.h"

class SendEquippedSkillCards : public LobbyServerPacket
{
public:
	explicit SendEquippedSkillCards(const std::array<InventoryCard, 3>& skillCards) : LobbyServerPacket(647)
	{
		for (const InventoryCard& skillCard : skillCards)
		{
			InventoryCardWriter::write(this, skillCard);
		}

		writeEmpty(22 * 43);
	}
};