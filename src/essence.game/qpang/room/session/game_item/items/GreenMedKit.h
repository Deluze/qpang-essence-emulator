#pragma once

#include "qpang/room/session/game_item/GameItem.h"

class GreenMedKit : public GameItem
{
public:
	uint32_t onPickUp(std::shared_ptr<RoomSessionPlayer> session) override
	{
		session->healTeam(50);

		return 0;
	}
};