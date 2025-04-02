#pragma once

#include <array>

#include "packets/LobbyServerPacket.h"

#include "qpang/player/Player.h"
#include "qpang/player/stats/StatsManager.h"

class SendAccountUpdate : public LobbyServerPacket
{
public:
	// TODO: Duplicate code in SendAccountAuthenticated.h
	explicit SendAccountUpdate(const Player::Ptr& player) : LobbyServerPacket(662)
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
		writeShort(300); // max inv. size
		writeShort(50); // max friend list size
		writeShort(10); // max inc/out friend list size
		writeShort(50); // max memo send count
		writeShort(50); // left over memo send count

		// player stats
		writeInt(1); // ?
		writeInt(player->getExperience());
		writeInt(player->getDon());
		writeInt(0); // ?

		const auto stats = player->getStatsManager();
		writeInt(stats->getKills());
		writeInt(stats->getDeaths());

		writeInt(stats->getNormalWins());
		writeInt(stats->getNormalLosses());
		writeInt(stats->getNormalDrews());
		writeInt(0); // ?
		writeInt(stats->getMissionWins());
		writeInt(stats->getMissionLosses());
		writeInt(stats->getMissionDrews());

		writeEmpty(72);
		writeInt(stats->getSlackerPoints());
		writeInt(player->getCoins());
		writeEmpty(44);

		const std::vector<uint16_t> unlockedCharacters = player->getEquipmentManager()->getUnlockedCharacters();

		for (const uint16_t unlockedCharacter : unlockedCharacters)
		{
			std::array<uint64_t, 9> armor = player->getEquipmentManager()->getArmorByCharacter(unlockedCharacter);
			std::array<uint64_t, 4> weapons = player->getEquipmentManager()->getWeaponsByCharacter(unlockedCharacter);

			writeShort(unlockedCharacter);

			for (const uint64_t armorCardId : armor)
			{
				writeLong(armorCardId);
			}

			for (const uint64_t weaponCardId : weapons)
			{
				writeLong(weaponCardId);
				writeLong(0); // ?
			}
		}

		writeEmpty(2455 - (138 * unlockedCharacters.size()));

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