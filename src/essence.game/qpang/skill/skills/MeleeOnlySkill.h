#pragma once

#include "Skill.h"

class MeleeOnlySkill final : public Skill
{
public:
	MeleeOnlySkill() : Skill(true, 15)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_MELEE_CHALLENGE;
	}

	uint32_t getSkillPointCost() override
	{
		return 3;
	}

	SkillRateType getSkillRateType() override
	{
		return SkillRateType::GOLD;
	}

	// Melee Only Skill
	bool shouldOnlyTakeMeleeDamage() override
	{
		return true;
	}

	// Melee Only Skill
	bool shouldOnlyDealMeleeDamage() override
	{
		return true;
	}
};
