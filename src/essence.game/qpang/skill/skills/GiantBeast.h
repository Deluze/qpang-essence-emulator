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
	}

	void apply() override
	{
		Skill::apply();

		// Set the health of the player to 600.
		m_player->setHealth(600);
	}

	uint32_t getItemId() override
	{
		return ItemID::SKILL_GIANT_BEAST;
	}
private:
};