#pragma once

class MentalUpSkill final : public Skill
{
public:
	MentalUpSkill() : Skill()
	{
		m_requiredSkillPoints = 2;

		m_hasDuration = true;
		m_durationInSeconds = 20;

		m_skillTarget = SkillTarget::SELF;
		m_isReflectableSkillCard = false;
	}

	uint32_t getItemId() override
	{
		return SKILL_MENTAL_UP;
	}

	bool shouldReceiveReducedDamageFromLaunchers() override
	{
		return true;
	}
};