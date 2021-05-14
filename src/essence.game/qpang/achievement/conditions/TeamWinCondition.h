#pragma once

#include <cstdint>

#include "qpang/achievement/conditions/AchievementCondition.h"
#include "qpang/player/Player.h"

class TeamWinCondition : public AchievementCondition
{
public:
	TeamWinCondition(uint32_t wins) :
		m_wins(wins)
	{

	}

	bool matches(const std::shared_ptr<RoomSessionPlayer>& player) override
	{
		return player->getPlayer()->getStatsManager()->getNormalWins() >= m_wins;
	}

private:
	uint32_t m_wins;
};