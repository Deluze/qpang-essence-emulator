#pragma once

#include "Skill.h"

class MeleeChallengeSkill final : public Skill
{
public:
	MeleeChallengeSkill() : Skill(true, 15)
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

	// Melee Challenge Skill
	bool shouldOnlyTakeMeleeDamage() override
	{
		return true;
	}

	// Melee Challenge Skill
	bool shouldOnlyDealMeleeDamage() override
	{
		return true;
	}
};
