#pragma once

class RapidFireSkill final : public Skill
{
public:
	RapidFireSkill() : Skill(true, 8)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_RAPID_FIRE;
	}

	uint32_t getSkillPointCost() override
	{
		return 1;
	}
};