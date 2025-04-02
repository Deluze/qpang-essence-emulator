#pragma once

class ShacklesSkill final : public Skill
{
public:
	ShacklesSkill() : Skill(true, 15)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_SHACKLES;
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

	bool isReflectableSkillCard() override
	{
		return true;
	}
};