#pragma once

#include <cstdint>

#include "qpang/achievement/conditions/AchievementCondition.h"

#include "qpang/player/Player.h"

class TeamDeathCondition : public AchievementCondition
{
public:
	TeamDeathCondition(uint32_t deaths) :
		m_deaths(deaths)
	{

	}

	bool matches(const std::shared_ptr<RoomSessionPlayer>& player) override
	{
		return player->getPlayer()->getStatsManager()->getTeamDeaths() >= m_deaths;
	}

private:
	uint32_t m_deaths;
};