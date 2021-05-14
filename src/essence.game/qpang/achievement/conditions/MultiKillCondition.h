#pragma once

#include <cstdint>

#include "qpang/achievement/conditions/AchievementCondition.h"

class MultiKillCondition : public AchievementCondition
{
public:
	MultiKillCondition(uint32_t kills) :
		m_kills(kills)
	{

	}

	bool matches(const std::shared_ptr<RoomSessionPlayer>& player) override
	{
		return player->getHighestMultiKill() >= m_kills;
	}

private:
	uint32_t m_kills;
};