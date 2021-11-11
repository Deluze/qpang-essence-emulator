#pragma once

class CamoSkill final : public Skill
{
public:
	CamoSkill() : Skill(true, 15)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_CAMO;
	}

	uint32_t getSkillPointCost() override
	{
		return 1;
	}

	SkillRateType getSkillRateType() override
	{
		return SkillRateType::BRONZE;
	}

	bool shouldDisableWhenDamageIsTaken() override
	{
		return true;
	}

	bool shouldDisableWhenGameItemIsPickedUp() override
	{
		return true;
	}

	bool shouldDisableWhenPerformingRollAction() override
	{
		return true;
	}
};