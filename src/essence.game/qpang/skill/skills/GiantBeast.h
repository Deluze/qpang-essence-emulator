#pragma once

#include <iostream>

class GiantBeastSkill : public Skill
{
public:
	GiantBeastSkill() : Skill()
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

		m_player->setHealth(m_giantBeastBaseHealth, true);
		m_player->makeInvincible(m_transformationTime);

		m_player->getWeaponManager()->equipRainbowSkillCardWeapon(m_giantBeastWeaponItemId);
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
		return ItemID::SKILL_GIANT_BEAST;
	}
private:
	uint32_t m_transformationTime = 9;
	uint32_t m_giantBeastBaseHealth = 600;
	uint32_t m_giantBeastWeaponItemId = 1095368707;
};