#pragma once

#include <iostream>

class TrapSkill : public Skill
{
public:
	TrapSkill() : Skill()
	{
		m_requiredSkillPoints = 1;

		m_hasDuration = true;
		m_durationInSeconds = 15;

		m_skillTarget = SkillTarget::ENEMY_PLAYER;
	}

	uint32_t getItemId() override
	{
		return ItemID::SKILL_TRAP;
	}
private:
};