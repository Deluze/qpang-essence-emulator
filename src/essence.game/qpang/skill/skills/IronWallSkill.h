#pragma once

class IronWallSkill final : public Skill
{
public:
	IronWallSkill() : Skill()
	{
		m_requiredSkillPoints = 1;

		m_hasDuration = true;
		m_durationInSeconds = 20;

		m_skillTarget = SkillTarget::SELF;
		m_isReflectableSkillCard = false;
	}

	uint32_t getItemId() override
	{
		return SKILL_IRON_WALL;
	}
};