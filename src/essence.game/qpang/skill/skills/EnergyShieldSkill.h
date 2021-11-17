#pragma once

#include "gc_card.hpp"
#include "Skill.h"

class EnergyShieldSkill final : public Skill
{
public:
	EnergyShieldSkill() : Skill(true, 8)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_ENERGY_SHIELD;
	}

	uint32_t getSkillPointCost() override
	{
		return 1;
	}

	SkillRateType getSkillRateType() override
	{
		return SkillRateType::BRONZE;
	}

	SkillTargetType getSkillTargetType() override
	{
		return SkillTargetType::SELF;
	}

	bool isEnergyShieldSkill() override
	{
		return true;
	}
};
