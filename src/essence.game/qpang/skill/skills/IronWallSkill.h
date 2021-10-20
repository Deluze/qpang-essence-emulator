#pragma once

#include "Skill.h"

class IronWallSkill final : public Skill
{
public:
	IronWallSkill() : Skill(true, 20)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_IRON_WALL;
	}

	uint32_t getSkillPointCost() override
	{
		return 1;
	}

	SkillRateType getSkillRateType() override
	{
		return SkillRateType::BRONZE;
	}
};
