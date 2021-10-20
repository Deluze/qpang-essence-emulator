#pragma once

#include "Skill.h"

class UndercoverSkill final : public Skill
{
public:
	UndercoverSkill() : Skill(true, 15)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_UNDERCOVER;
	}

	uint32_t getSkillPointCost() override
	{
		return 1;
	}

	SkillTargetType getSkillTargetType() override
	{
		return SkillTargetType::ALLY;
	}

	SkillRateType getSkillRateType() override
	{
		return SkillRateType::BRONZE;
	}
};
