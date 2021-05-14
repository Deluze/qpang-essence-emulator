#pragma once

#include <cstdint>

#include "qpang/achievement/conditions/AchievementCondition.h"
#include "qpang/player/Player.h"

class DeathCondition : public AchievementCondition
{
public:
	DeathCondition(uint32_t deaths) :
		m_deaths(deaths)
	{

	}

	bool matches(const std::shared_ptr<RoomSessionPlayer>& player) override
	{
		return player->getPlayer()->getStatsManager()->getDeaths() >= m_deaths;
	}

private:
	uint32_t m_deaths;
};