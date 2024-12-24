#pragma once

#include "gc_card.hpp"
#include "Skill.h"

class BlessingSkill final : public Skill
{
public:
	BlessingSkill() : Skill(true, 10)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_BLESSING;
	}

	uint32_t getSkillPointCost() override
	{
		return 3;
	}

	SkillRateType getSkillRateType() override
	{
		return SkillRateType::GOLD;
	}

	SkillTargetType getSkillTargetType() override
	{
		return SkillTargetType::ALLY_TEAM;
	}
};
