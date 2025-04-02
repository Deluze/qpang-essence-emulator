#pragma once

#include "Skill.h"

class TeamCheerSkill final : public Skill
{
public:
	TeamCheerSkill() : Skill(true, 15)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_TEAM_CHEER;
	}

	uint32_t getSkillPointCost() override
	{
		return 2;
	}

	SkillRateType getSkillRateType() override
	{
		return SkillRateType::SILVER;
	}

	SkillTargetType getSkillTargetType() override
	{
		return SkillTargetType::ALLY_TEAM;
	}
};
