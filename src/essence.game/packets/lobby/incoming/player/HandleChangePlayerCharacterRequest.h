#pragma once

#include <cstdint>
#include <vector>

#include "core/communication/packet/PacketEvent.h"
#include "qpang/player/Player.h"

class HandleChangePlayerCharacterRequest final : public PacketEvent
{
public:
	void handle(const QpangConnection::Ptr conn, QpangPacket& pack) override
	{
		const auto characterId = pack.readShort();

		if (const auto player = conn->getPlayer(); player != nullptr)
		{
			const std::vector<uint16_t> characters = player->getEquipmentManager()->getUnlockedCharacters();

			if (const auto it = std::find(characters.cbegin(), characters.cend(), characterId); it != characters.cend())
			{
				player->setCharacter(characterId);
			}
		}
	}
};
