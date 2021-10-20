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

	bool shouldDisableOnDamageReceive() override
	{
		return true;
	}

	bool shouldDisableOnGameItemPickup() override
	{
		return true;
	}

	bool shouldDisableOnRollAction() override
	{
		return true;
	}
};