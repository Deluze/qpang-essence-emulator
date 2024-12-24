#pragma once

class SkillSealSkill final : public Skill
{
public:
	SkillSealSkill() : Skill(true, 15)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_SKILL_SEAL;
	}

	uint32_t getSkillPointCost() override
	{
		return 2;
	}

	SkillRateType getSkillRateType() override
	{
		return SkillRateType::SILVER;
	}

	SkillTargetType getSkillTargetType() override
	{
		return SkillTargetType::ENEMY;
	}
};