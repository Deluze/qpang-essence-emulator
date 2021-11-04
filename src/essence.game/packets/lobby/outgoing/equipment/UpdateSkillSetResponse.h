#pragma once

#include <array>

#include "InventoryCard.h"
#include "InventoryCardWriter.h"
#include "packets/LobbyServerPacket.h"

class UpdateSkillSetResponse : public LobbyServerPacket
{
public:
	explicit UpdateSkillSetResponse(const std::array<InventoryCard, 3>& skillCards) : LobbyServerPacket(650)
	{
		for (const InventoryCard& skillCard : skillCards)
		{
			InventoryCardWriter::write(this, skillCard);
		}

		writeEmpty(22 * 43);
	}
};