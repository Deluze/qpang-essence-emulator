#pragma once

#include <iostream>

class PocketSizeSkill : public Skill
{
public:
	PocketSizeSkill() : Skill()
	{
		m_requiredSkillPoints = 2;

		m_hasDuration = true;
		m_durationInSeconds = 15;

		m_skillTarget = SkillTarget::SELF;
		m_isReflectableSkillCard = false;
	}

	uint32_t getItemId() override
	{
		return ItemID::SKILL_POCKET_SIZE;
	}

	void onApply() override
	{
		Skill::onApply();

		const auto currentHealth = m_player->getHealth();
		const auto bonusHealth = (uint16_t) currentHealth / 2;
		
		m_player->setHealth((currentHealth + bonusHealth), true);
	}

	void onWearOff() override
	{
	}
private:
};