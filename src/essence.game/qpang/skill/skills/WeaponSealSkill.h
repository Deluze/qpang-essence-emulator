#pragma once

class WeaponSealSkill final : public Skill
{
public:
	WeaponSealSkill() : Skill()
	{
		m_requiredSkillPoints = 2;

		m_hasDuration = true;
		m_durationInSeconds = 10;

		m_skillTarget = SkillTarget::ENEMY_PLAYER;
		m_isReflectableSkillCard = true;
	}

	uint32_t getItemId() override
	{
		return SKILL_WEAPON_SEAL;
	}
};