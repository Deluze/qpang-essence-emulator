#pragma once

#include <iostream>

class ShacklesSkill : public Skill
{
public:
	ShacklesSkill() : Skill()
	{
		m_requiredSkillPoints = 1;

		m_hasDuration = true;
		m_durationInSeconds = 15;

		m_skillTarget = SkillTarget::ENEMY_PLAYER;

		m_isReflectableSkillCard = true;
		// NOTE: Since the client does not send an actioncard event to the server whilst the target player is "shackled",
		// we won't have to check in CGCard if the player is shackled at that point in time (all clientside behavior).

		m_isAvailableInMeleeOnlyMode = true;
	}

	uint32_t getItemId() override
	{
		return ItemID::SKILL_SHACKLES;
	}
private:
};