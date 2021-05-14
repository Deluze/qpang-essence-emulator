#pragma once

#include <cstdint>

#include "qpang/achievement/conditions/AchievementCondition.h"

#include "qpang/player/Player.h"

class HeadshotDeathCondition : public AchievementCondition
{
public:
	HeadshotDeathCondition(uint32_t deaths) :
		m_deaths(deaths)
	{

	}

	bool matches(const std::shared_ptr<RoomSessionPlayer>& player) override
	{
		return player->getPlayer()->getStatsManager()->getHeadshotDeaths() >= m_deaths;
	}

private:
	uint32_t m_deaths;
};