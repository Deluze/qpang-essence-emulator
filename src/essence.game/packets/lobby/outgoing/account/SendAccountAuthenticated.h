#pragma once

#include <array>

#include "packets/LobbyServerPacket.h"
#include "qpang/player/Player.h"
#include "qpang/player/stats/StatsManager.h"

class SendAccountAuthenticated : public LobbyServerPacket
{
public:
	explicit SendAccountAuthenticated(const Player::Ptr& player) : LobbyServerPacket(601)
	{
		// Player info
		writeInt(player->getId());										// Player id
		writeEmpty(42);													// Empty
		// TODO: Send empty string as name if player has not fully registered yet.
		writeString(player->getName(), 16);								// Player name
		writeInt(player->getStatsManager()->getPlayTimeInMinutes());	// Playtime in minutes.
		writeInt(player->getCash());									// Cash
		writeInt(player->getRank());									// Rank
		writeShort(0); // ?												// Unknown
		writeShort(player->getCharacter());								// Character id
		writeInt(player->getLevel());									// Level
		writeEmpty(8);													// Empty

		// Player settings
		writeFlag(true);	// accept pm
		writeFlag(true);	// accept game invites
		writeFlag(true);	// accept friend invites
		writeFlag(true);	// unknown
		writeFlag(true);	// accept trade requests
		writeEmpty(20);	// empty
		writeByte(4);	// current tutorial stage
		writeEmpty(12);	// empty

		// Player containers
		writeShort(200);	// max inv. size
		writeShort(50);	// max friend list size
		writeShort(10);	// max inc/out friend list size
		writeShort(50);	// max memo send count
		writeShort(50);	// left over memo send count

		// Player statistics
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
