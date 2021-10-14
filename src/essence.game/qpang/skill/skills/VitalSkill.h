#pragma once

#include <iostream>

class VitalSkill : public Skill
{
public:
	VitalSkill() : Skill()
	{
		m_requiredSkillPoints = 3;

		m_hasDuration = true;
		m_durationInSeconds = 10;

		m_skillTarget = SkillTarget::ALL_TEAM_PLAYERS;
	}

	void tick() override
	{
		Skill::tick();

		if (const auto roomSession = m_player->getRoomSession(); roomSession != nullptr)
		{
			const auto teamPlayers = roomSession->getPlayersForTeam(m_player->getTeam());

			// Iterate over all team players that are alive and give them 5 health each tick for the entire duration of the skillcard.
			for (const auto &teamPlayer : teamPlayers)
			{
				if (!teamPlayer->isDead())
				{
					teamPlayer->addHealth(5, true);
				}
			}

		}
	}

	void apply() override
	{
		Skill::apply();

		// Set the health of the player to 1.
		m_player->setHealth(1);
	}

	uint32_t getItemId() override
	{
		return ItemID::SKILL_VITAL;
	}
private:
};