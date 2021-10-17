#pragma once

#include <iostream>

class SkillSealSkill : public Skill
{
public:
	SkillSealSkill() : Skill()
	{
		m_requiredSkillPoints = 2;

		m_hasDuration = true;
		m_durationInSeconds = 15;

		m_skillTarget = SkillTarget::ENEMY_PLAYER;
		m_isReflectableSkillCard = false;

		m_isAvailableInMeleeOnlyMode = true;
	}

	uint32_t getItemId() override
	{
		return ItemID::SKILL_SKILL_SEAL;
	}
private:
};