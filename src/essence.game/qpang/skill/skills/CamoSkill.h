#pragma once

class CamoSkill final : public Skill
{
public:
	CamoSkill() : Skill()
	{
		m_requiredSkillPoints = 1;

		m_hasDuration = true;
		m_durationInSeconds = 15;

		m_skillTarget = SkillTarget::SELF;
		m_isReflectableSkillCard = false;
	}

	uint32_t getItemId() override
	{
		return SKILL_CAMO;
	}

	bool shouldDisableOnDamageReceive() override
	{
		return true;
	}

	bool shouldDisableOnGameItemPickup() override
	{
		return true;
	}

	bool shouldDisableOnRollAction() override
	{
		return true;
	}
};