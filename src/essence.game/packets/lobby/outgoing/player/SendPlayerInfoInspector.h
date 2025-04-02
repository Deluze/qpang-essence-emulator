#pragma once

#include "packets/LobbyServerPacket.h"
#include "qpang/player/Player.h"
#include "qpang/player/stats/StatsManager.h"

class SendPlayerInfoInspector : public LobbyServerPacket
{
public:
	explicit SendPlayerInfoInspector(const Player::Ptr& player) : LobbyServerPacket(692)
	{
		writeInt(player->getId());
		writeEmpty(42);
		writeString(player->getName(), 16);
		writeInt(player->getStatsManager()->getPlayTime() / 60);
		writeEmpty(4);
		writeByte(player->getLevel());
		writeByte(player->getRank());
		writeShort(player->getCharacter());

		const auto armor = player->getEquipmentManager()->getArmorItemIdsByCharacter(player->getCharacter());

		for (const auto& armorId : armor)
		{
			writeInt(armorId);
		}

		const auto weapons = player->getEquipmentManager()->getWeaponItemIdsByCharacter(player->getCharacter());

		for (const auto& weaponId : weapons)
		{
			writeInt(weaponId);
			writeInt(0);
		}

		writeEmpty(40);
		writeInt(player->getExperience());
		writeInt(1);
		writeInt(player->getStatsManager()->getKills());
		writeInt(player->getStatsManager()->getDeaths());
		writeInt(player->getStatsManager()->getNormalWins());
		writeInt(player->getStatsManager()->getNormalLosses());
		writeInt(player->getStatsManager()->getNormalDrews());
		writeInt(0);
		writeInt(player->getStatsManager()->getMissionWins());
		writeInt(player->getStatsManager()->getMissionLosses());
		writeInt(player->getStatsManager()->getMissionDrews());
		writeShort(player->getStatsManager()->getSlackerPoints());
		writeInt(0); // ?
		writeInt(1);
		
		const auto [rank, difference] = Game::instance()->getLeaderboard()->getPosition(player->getId());

		writeInt(rank);
		writeInt(difference);

		const auto achievements = player->getAchievementContainer()->list();

		writeByte(static_cast<uint8_t>(achievements.size()));

		for (const auto& achievement : achievements)
		{
			writeInt(achievement); // title id
			writeInt(1); // ? belongs to title
		}

		writeEmpty(1000);
	}
};
