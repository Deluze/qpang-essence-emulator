#pragma once

#include <iostream>

class LastWeaponSkill : public Skill
{
public:
	LastWeaponSkill() : Skill()
	{
		m_requiredSkillPoints = 4;

		m_hasDuration = true;
		m_durationInSeconds = 40;

		m_skillTarget = SkillTarget::SELF;

		m_isRainbowSkillCard = true;
	}

	void onApply() override
	{
		Skill::onApply();

		m_player->setHealth(m_lastWeaponBaseHealth, true);
		m_player->makeInvincible(m_transformationTime);

		m_player->getWeaponManager()->equipRainbowSkillCardWeapon(m_lastWeaponWeaponItemId);
	}

	void onWearOff() override
	{
		if (!m_player->isDead())
		{
			m_player->setHealth(m_player->getDefaultHealth(), true);
		}

		m_player->getWeaponManager()->unequipRainbowSkillCardWeapon();
	}

	uint32_t getItemId() override
	{
		return ItemID::SKILL_LAST_WEAPON;
	}
private:
	uint32_t m_transformationTime = 6;

	uint32_t m_lastWeaponBaseHealth = 2000;
	uint32_t m_lastWeaponWeaponItemId = 1095434243;
};