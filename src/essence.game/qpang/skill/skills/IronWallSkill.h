#pragma once

#include <iostream>

class IronWallSkill : public Skill
{
public:
	IronWallSkill() : Skill()
	{
		m_requiredSkillPoints = 1;

		m_hasDuration = true;
		m_durationInSeconds = 20;

		m_skillTarget = SkillTarget::SELF;
	}

	uint32_t getItemId() override
	{
		return ItemID::SKILL_IRON_WALL;
	}
private:
};