#pragma once

class SkillSealSkill final : public Skill
{
public:
	SkillSealSkill() : Skill()
	{
		m_requiredSkillPoints = 2;

		m_hasDuration = true;
		m_durationInSeconds = 15;

		m_skillTarget = SkillTarget::ENEMY_PLAYER;
		m_isReflectableSkillCard = false;
	}

	uint32_t getItemId() override
	{
		return SKILL_SKILL_SEAL;
	}
};