#pragma once

#include <iostream>

class TranseSkill : public Skill
{
public:
	TranseSkill() : Skill()
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

		m_player->setHealth(m_transeBaseHealth, true);
		m_player->makeInvincible(m_transformationTime);

		m_player->getWeaponManager()->equipRainbowSkillCardWeapon(m_transeWeaponItemId);
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
		return ItemID::SKILL_TRANSE;
	}
private:
	uint32_t m_transformationTime = 6;
	uint32_t m_transeBaseHealth = 500;
	uint32_t m_transeWeaponItemId = 1095303170;
};