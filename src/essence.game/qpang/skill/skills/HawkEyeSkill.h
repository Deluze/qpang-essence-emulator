#pragma once

#include "Skill.h"

class HawkEyeSkill final : public Skill
{
public:
	HawkEyeSkill() : Skill(true, 5)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_HAWK_EYE;
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
