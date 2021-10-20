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

	SkillTargetType getSkillTarget() override
	{
		return SkillTargetType::ENEMY;
	}

	bool isReflectableSkillCard() override
	{
		return true;
	}
};