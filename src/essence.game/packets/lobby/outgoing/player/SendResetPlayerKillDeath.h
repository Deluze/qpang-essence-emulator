#pragma once

#include "packets/LobbyServerPacket.h"
#include "qpang/player/Player.h"
#include "qpang/player/stats/StatsManager.h"

class SendResetPlayerKillDeath : public LobbyServerPacket
{
public:
	SendResetPlayerKillDeath(const Player::Ptr& player, const InventoryCard& card) : LobbyServerPacket(845)
	{
		writeInt(0);
		InventoryCardWriter::write(this, card);
		writeInt(player->getExperience());
		writeInt(player->getDon());
		writeInt(0);
		writeInt(0); 
		writeInt(0);
		writeInt(player->getStatsManager()->getNormalWins());
		writeInt(player->getStatsManager()->getNormalLosses());
		writeInt(player->getStatsManager()->getNormalDrews());
		writeInt(0);
		writeInt(player->getStatsManager()->getMissionWins());
		writeInt(player->getStatsManager()->getMissionLosses());
		writeInt(player->getStatsManager()->getMissionDrews());
		writeEmpty(72);
		writeInt(player->getStatsManager()->getSlackerPoints());
	}
};