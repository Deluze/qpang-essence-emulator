#pragma once

#include <array>

#include "packets/LobbyServerPacket.h"

#include "qpang/player/Player.h"
#include "qpang/player/stats/StatsManager.h"

class Authenticated : public LobbyServerPacket
{
public:
	Authenticated(Player::Ptr player) : LobbyServerPacket(601)
	{
		// player info
		writeInt(player->getId());
		writeEmpty(42);
		writeString(player->getName(), 16);
		writeInt(player->getStatsManager()->getPlayTimeInMinutes());
		writeInt(player->getCash());
		writeInt(player->getRank());
		writeShort(0); // ?
		writeShort(player->getCharacter());
		writeInt(player->getLevel());
		writeEmpty(8);

		// player settings
		writeFlag(true); // accept pm
		writeFlag(true); // accept game invites
		writeFlag(true); // accept friend invites
		writeFlag(true); // unknown
		writeFlag(true); // accept trade requests
		writeEmpty(20);
		writeByte(4); // current tutorial stage
		writeEmpty(12);

		// player containers
		writeShort(200); // max inv. size
		writeShort(50); // max friend list size
		writeShort(10); // max inc/out friend list size
		writeShort(50); // max memo send count
		writeShort(50); // left over memo send count

		// player stats
		writeInt(1); // ?
		writeInt(player->getExperience());
		writeInt(player->getDon());
		writeInt(0); // ?
		writeInt(player->getStatsManager()->getKills());
		writeInt(player->getStatsManager()->getDeaths());

		writeInt(player->getStatsManager()->getNormalWins());
		writeInt(player->getStatsManager()->getNormalLosses());
		writeInt(player->getStatsManager()->getNormalDrews());
		writeInt(0); // ?
		writeInt(player->getStatsManager()->getMissionWins());
		writeInt(player->getStatsManager()->getMissionLosses());
		writeInt(player->getStatsManager()->getMissionDrews());

		writeEmpty(72);
		writeInt(player->getStatsManager()->getSlackerPoints());
		writeInt(player->getCoins());
		writeEmpty(44);

		std::vector<uint16_t> characters = player->getEquipmentManager()->getUnlockedCharacters();
		for (uint16_t character : characters)
		{
			std::array<uint64_t, 9> armor = player->getEquipmentManager()->getArmorByCharacter(character);
			std::array<uint64_t, 4> weapons = player->getEquipmentManager()->getWeaponsByCharacter(character);

			writeShort(character);

			for (uint64_t armorCardId : armor)
				writeLong(armorCardId);

			for (uint64_t weaponCardId : weapons)
			{
				writeLong(weaponCardId);
				writeLong(0); // ?
			}
		}

		writeEmpty(2455 - (138 * characters.size()));

		const auto achievements = player->getAchievementContainer()->list();

		writeShort(static_cast<uint16_t>(achievements.size()));
		for (const auto& achievement : achievements)
		{
			writeInt(achievement);
			writeInt(0);
		}
		
		writeEmpty(1635 - achievements.size() * 8);
	}
};