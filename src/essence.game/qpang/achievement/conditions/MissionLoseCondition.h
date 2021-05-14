#pragma once

#include <cstdint>

#include "qpang/achievement/conditions/AchievementCondition.h"
#include "qpang/player/Player.h"

class MissionLoseCondition : public AchievementCondition
{
public:
	MissionLoseCondition(uint32_t losses) :
		m_losses(losses)
	{

	}

	bool matches(const std::shared_ptr<RoomSessionPlayer>& player) override
	{
		return player->getPlayer()->getStatsManager()->getMissionLosses() >= m_losses;
	}

private:
	uint32_t m_losses;
};