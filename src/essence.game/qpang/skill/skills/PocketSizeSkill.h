#pragma once

#include <iostream>

class PocketSizeSkill : public Skill
{
public:
	PocketSizeSkill() : Skill()
	{
		m_requiredSkillPoints = 2;

		m_hasDuration = true;
		m_durationInSeconds = 15;

		m_skillTarget = SkillTarget::SELF;
	}

	uint32_t getItemId() override
	{
		return ItemID::SKILL_POCKET_SIZE;
	}
private:
};