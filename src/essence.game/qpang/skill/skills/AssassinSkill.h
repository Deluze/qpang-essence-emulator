#pragma once

class AssassinSkill final : public Skill
{
public:
	AssassinSkill() : Skill(false, 99)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_ASSASSIN;
	}

	uint32_t getSkillPointCost() override
	{
		return 2;
	}

	SkillRateType getSkillRateType() override
	{
		return SkillRateType::SILVER;
	}

	bool isAssassinSkill() override
	{
		return true;
	}
};