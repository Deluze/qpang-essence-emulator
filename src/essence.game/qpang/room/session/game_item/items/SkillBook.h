#pragma once

#include "qpang/room/session/game_item/GameItem.h"

class SkillBook : public GameItem
{
public:
	uint32_t onPickUp(RoomSessionPlayer::Ptr session) override
	{
		return session->getSkillManager()->drawSkill();
	}
};