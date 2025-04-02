#pragma once

class ChaosSkill final : public Skill
{
public:
	ChaosSkill() : Skill(true, 15)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_CHAOS;
	}

	uint32_t getSkillPointCost() override
	{
		return 1;
	}

	SkillRateType getSkillRateType() override
	{
		return SkillRateType::BRONZE;
	}

	bool isReflectableSkillCard() override
	{
		return true;
	}

	SkillTargetType getSkillTargetType() override
	{
		return SkillTargetType::ENEMY;
	}
};