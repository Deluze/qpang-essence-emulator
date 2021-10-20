#pragma once

class RageSkill final : public Skill
{
public:
	RageSkill() : Skill(true, 15)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_RAGE;
	}

	uint32_t getSkillPointCost() override
	{
		return 1;
	}

	SkillRateType getSkillRateType() override
	{
		return SkillRateType::BRONZE;
	}

	SkillTargetType getSkillTarget() override
	{
		return SkillTargetType::ENEMY;
	}

	bool isReflectableSkillCard() override
	{
		return true;
	}
};