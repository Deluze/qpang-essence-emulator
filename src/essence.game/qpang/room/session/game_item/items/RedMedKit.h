#pragma once

#include "qpang/room/session/game_item/GameItem.h"

class RedMedKit : public GameItem
{
public:
	uint32_t onPickUp(std::shared_ptr<RoomSessionPlayer> session) override
	{
		session->addHealth(50, true);

		return 0;
	}
};