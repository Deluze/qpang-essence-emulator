#pragma once

#include "Skill.h"

class StuntUpSkill final : public Skill
{
public:
	StuntUpSkill() : Skill(true, 30)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_STUNT_UP;
	}

	uint32_t getSkillPointCost() override
	{
		return 2;
	}

	SkillRateType getSkillRateType() override
	{
		return SkillRateType::SILVER;
	}

	bool shouldReceiveReducedDamageFromAllSources() override
	{
		return true;
	}
};
