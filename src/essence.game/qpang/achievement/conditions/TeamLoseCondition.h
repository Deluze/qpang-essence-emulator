#pragma once

#include <cstdint>

#include "qpang/achievement/conditions/AchievementCondition.h"
#include "qpang/player/Player.h"

class TeamLoseCondition : public AchievementCondition
{
public:
	TeamLoseCondition(uint32_t losses) :
		m_losses(losses)
	{

	}

	bool matches(const std::shared_ptr<RoomSessionPlayer>& player) override
	{
		return player->getPlayer()->getStatsManager()->getNormalLosses() >= m_losses;
	}

private:
	uint32_t m_losses;
};