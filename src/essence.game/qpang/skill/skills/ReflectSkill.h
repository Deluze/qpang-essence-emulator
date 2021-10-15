#pragma once

#include <iostream>

class ReflectSkill : public Skill
{
public:
	ReflectSkill() : Skill()
	{
		m_requiredSkillPoints = 2;

		m_hasDuration = false;
		m_durationInSeconds = 30;

		m_skillTarget = SkillTarget::SELF;
		m_isReflectableSkillCard = false;
	}

	uint32_t getItemId() override
	{
		return ItemID::SKILL_REFLECT;
	}

	bool shouldReflectSkillCard() override
	{
		return true;
	}
private:
};