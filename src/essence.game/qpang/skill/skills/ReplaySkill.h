#pragma once

#include "Skill.h"

class ReplaySkill final : public Skill
{
public:
	ReplaySkill() : Skill(false, 30)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_REPLAY;
	}

	uint32_t getSkillPointCost() override
	{
		return 2;
	}

	SkillRateType getSkillRateType() override
	{
		return SkillRateType::SILVER;
	}

	bool shouldInstantlyRespawnWhenPlayerHasDied() override
	{
		return true;
	}
};
