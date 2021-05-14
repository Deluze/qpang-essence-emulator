#pragma once

#include <memory>

#include "qpang/room/session/player/RoomSessionPlayer.h"

class AchievementCondition
{
public:
	virtual bool matches(const std::shared_ptr<RoomSessionPlayer>& player)
	{
		return false;
	}
};