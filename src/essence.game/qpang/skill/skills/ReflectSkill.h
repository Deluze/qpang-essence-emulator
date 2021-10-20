#pragma once

class ReflectSkill final : public Skill
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
		return SKILL_REFLECT;
	}

	bool shouldReflectSkillCard() override
	{
		return true;
	}
};