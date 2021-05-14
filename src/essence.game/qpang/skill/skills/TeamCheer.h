#pragma once

#include <iostream>

#include "qpang/skill/Skill.h"

class TeamCheer : public Skill
{
public:
	TeamCheer() = default;

	uint32_t getId() override
	{
		return ItemID::SKILL_TEAM_CHEER;
	}
};