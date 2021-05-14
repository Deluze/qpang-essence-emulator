#pragma once

#include <memory>

#include "qpang/room/session/player/RoomSessionPlayer.h"

class GameItem
{
public:
	virtual uint32_t onPickUp(std::shared_ptr<RoomSessionPlayer> player)
	{
		return 0;
	}
};