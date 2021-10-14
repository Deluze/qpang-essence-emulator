#pragma once

#include <iostream>

class ShotMoveSkill : public Skill
{
public:
	ShotMoveSkill() : Skill()
	{
		m_requiredSkillPoints = 1;

		m_hasDuration = true;
		m_durationInSeconds = 8;

		m_skillTarget = SkillTarget::SELF;
	}

	uint32_t getItemId() override
	{
		return ItemID::SKILL_SHOT_MOVE;
	}
private:
};