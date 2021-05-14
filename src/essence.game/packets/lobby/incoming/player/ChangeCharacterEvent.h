#pragma once

#include <vector>
#include <cstdint>

#include "core/communication/packet/PacketEvent.h"

#include "qpang/player/Player.h"

class ChangeCharacterEvent : public PacketEvent
{
public:
	void handle(QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		auto characterId = pack.readShort();

		if (const auto player = conn->getPlayer(); player != nullptr)
		{
			std::vector<uint16_t> characters = player->getEquipmentManager()->getUnlockedCharacters();
			auto it = std::find(characters.cbegin(), characters.cend(), characterId);

			if (it != characters.cend())
				player->setCharacter(characterId);
		}
	}
};