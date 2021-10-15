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

	void onApply() override
	{
		Skill::onApply();

		// TODO: Save health before transforming.
		// TODO: Save currently held weapon before transforming.
		// TODO: Change weapon to weapon for zilla.
		// 
		// Set the health of the player to 600.
		m_player->setHealth(600);
	}

	void onWearOff() override
	{
		// TODO: Set health back to what it was?
		// TODO: Set weapon back to previous weapon.
	}

	uint32_t getItemId() override
	{
		return ItemID::SKILL_GIANT_BEAST;
	}
private:
};