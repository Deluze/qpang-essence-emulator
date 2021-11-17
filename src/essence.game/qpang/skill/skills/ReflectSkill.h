#pragma once

class ReflectSkill final : public Skill
{
public:
	ReflectSkill() : Skill(false, 30)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_REFLECT;
	}

	uint32_t getSkillPointCost() override
	{
		return 2;
	}

	SkillRateType getSkillRateType() override
	{
		return SkillRateType::SILVER;
	}

	bool isReflectSkill() override
	{
		return true;
	}
};