#pragma once

#include <iostream>

class HawkEyeSkill : public Skill
{
public:
	HawkEyeSkill() : Skill()
	{
		m_requiredSkillPoints = 1;

		m_hasDuration = true;
		m_durationInSeconds = 5;
	}

	uint32_t getItemId() override
	{
		return ItemID::SKILL_HAWK_EYE;
	}
private:
};