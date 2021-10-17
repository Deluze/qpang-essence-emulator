#pragma once

#include <iostream>

class WeaponSealSkill : public Skill
{
public:
	WeaponSealSkill() : Skill()
	{
		m_requiredSkillPoints = 2;

		m_hasDuration = true;
		m_durationInSeconds = 10;

		m_skillTarget = SkillTarget::ENEMY_PLAYER;
		m_isReflectableSkillCard = true;

		m_isAvailableInMeleeOnlyMode = false;
	}

	uint32_t getItemId() override
	{
		return ItemID::SKILL_WEAPON_SEAL;
	}
private:
};