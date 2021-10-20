#pragma once

class MentalUpSkill final : public Skill
{
public:
	MentalUpSkill() : Skill(true, 20)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_MENTAL_UP;
	}

	uint32_t getSkillPointCost() override
	{
		return 2;
	}

	SkillRateType getSkillRateType() override
	{
		return SkillRateType::SILVER;
	}

	bool shouldReceiveReducedDamageFromLaunchers() override
	{
		return true;
	}
};