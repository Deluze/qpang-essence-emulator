#pragma once

#include <cstdint>

#include "qpang/achievement/conditions/AchievementCondition.h"

class StreakCondition : public AchievementCondition
{
public:
	StreakCondition(uint32_t streak) :
		m_streak(streak)
	{

	}

	bool matches(const std::shared_ptr<RoomSessionPlayer>& player) override
	{
		return player->getHighestStreak() >= m_streak;
	}

private:
	uint32_t m_streak;
};