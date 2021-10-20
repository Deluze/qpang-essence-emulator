#pragma once

class ChaosSkill final : public Skill
{
public:
	ChaosSkill() : Skill()
	{
		m_requiredSkillPoints = 1;

		m_hasDuration = true;
		m_durationInSeconds = 15;

		m_skillTarget = SkillTarget::ENEMY_PLAYER;
		m_isReflectableSkillCard = true;
	}

	uint32_t getItemId() override
	{
		return SKILL_CHAOS;
	}
};