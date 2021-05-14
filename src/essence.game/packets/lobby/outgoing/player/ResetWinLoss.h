#pragma once

#include <string>

#include "qpang/player/Player.h"
#include "qpang/player/stats/StatsManager.h"
#include "packets/LobbyServerPacket.h"

class ResetWinLoss : public LobbyServerPacket
{
public:
	ResetWinLoss(const Player::Ptr& player, InventoryCard& card) : LobbyServerPacket(842)
	{
		writeInt(0);
		InventoryCardWriter::write(this, card);
		writeInt(player->getExperience());
		writeInt(player->getDon());
		writeInt(0);
		writeInt(player->getStatsManager()->getKills());
		writeInt(player->getStatsManager()->getDeaths());
		writeInt(0);
		writeInt(0);
		writeInt(0);
		writeInt(0);
		writeInt(0);
		writeInt(0);
		writeInt(0);
		writeEmpty(72);
		writeInt(player->getStatsManager()->getSlackerPoints());
	}
};