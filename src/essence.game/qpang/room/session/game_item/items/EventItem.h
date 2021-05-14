#pragma once

#include "qpang/room/session/game_item/GameItem.h"

class EventItem : public GameItem
{
public:
	uint32_t onPickUp(std::shared_ptr<RoomSessionPlayer> session) override
	{
		session->addEventItemPickUp();

		return 0;
	}
};