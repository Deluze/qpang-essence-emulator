#pragma once

#include <iostream>

class AssassinSkill : public Skill
{
public:
	AssassinSkill() : Skill()
	{
		m_requiredSkillPoints = 2;

		m_hasDuration = false;
		m_durationInSeconds = 99;

		m_skillTarget = SkillTarget::SELF;
		m_isReflectableSkillCard = false;
	}

	uint32_t getItemId() override
	{
		return ItemID::SKILL_ASSASSIN;
	}

	bool shouldInstantlyKillEnemyWithMeleeWeapon() override
	{
		return true;
	}
private:
};