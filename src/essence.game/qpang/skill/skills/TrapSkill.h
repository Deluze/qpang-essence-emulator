#pragma once

class TrapSkill final : public Skill
{
public:
	TrapSkill() : Skill(true, 15)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_TRAP;
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
		return SkillTargetType::ENEMY;
	}
};