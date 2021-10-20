#pragma once

class HawkEyeSkill final : public Skill
{
public:
	HawkEyeSkill() : Skill()
	{
		m_requiredSkillPoints = 1;

		m_hasDuration = true;
		m_durationInSeconds = 5;

		m_skillTarget = SkillTarget::SELF;
		m_isReflectableSkillCard = false;
	}

	uint32_t getItemId() override
	{
		return SKILL_HAWK_EYE;
	}
};