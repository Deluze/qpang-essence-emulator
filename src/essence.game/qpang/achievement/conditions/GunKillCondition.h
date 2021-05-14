#pragma once

#include <cstdint>

#include "qpang/achievement/conditions/AchievementCondition.h"

#include "qpang/player/Player.h"

class GunKillCondition : public AchievementCondition
{
public:
	GunKillCondition(uint32_t kills) :
		m_kills(kills)
	{

	}

	bool matches(const std::shared_ptr<RoomSessionPlayer>& player) override
	{
		return player->getPlayer()->getStatsManager()->getGunKills() >= m_kills;
	}

private:
	uint32_t m_kills;
};