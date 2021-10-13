#pragma once

#include <iostream>

class CamoSkill : public Skill
{
public:
	CamoSkill() : Skill()
	{
		m_requiredSkillPoints = 1;

		m_hasDuration = true;
		m_durationInSeconds = 15;
	}

	uint32_t getItemId() override
	{
		return ItemID::SKILL_CAMO;
	}

	bool shouldDisableOnBeingAttacked() override
	{
		return true;
	}

	bool shouldDisableOnGameItemPickup() override
	{
		return true;
	}

	bool shouldDisableOnUseOfActionCard() override
	{
		return true;
	}
private:
};