#pragma once

class TrapSkill final : public Skill
{
public:
	TrapSkill() : Skill()
	{
		m_requiredSkillPoints = 1;

		m_hasDuration = true;
		m_durationInSeconds = 15;

		m_skillTarget = SkillTarget::ENEMY_PLAYER;
		m_isReflectableSkillCard = false;
	}

	uint32_t getItemId() override
	{
		return SKILL_TRAP;
	}
};