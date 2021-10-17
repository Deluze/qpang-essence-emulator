#pragma once

#include <iostream>

class RageSkill : public Skill
{
public:
	RageSkill() : Skill()
	{
		m_requiredSkillPoints = 1;

		m_hasDuration = true;
		m_durationInSeconds = 15;

		m_skillTarget = SkillTarget::ENEMY_PLAYER;

		m_isReflectableSkillCard = true;

		m_isAvailableInMeleeOnlyMode = false;
	}

	uint32_t getItemId() override
	{
		return ItemID::SKILL_RAGE;
	}
private:
};