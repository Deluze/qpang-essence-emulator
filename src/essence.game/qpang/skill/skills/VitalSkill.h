#pragma once

class VitalSkill final : public Skill
{
public:
	VitalSkill() : Skill()
	{
		m_requiredSkillPoints = 3;

		m_hasDuration = true;
		m_durationInSeconds = 10;

		m_skillTarget = SkillTarget::ALL_TEAM_PLAYERS;
		m_isReflectableSkillCard = false;
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
				const auto teamPlayerId = teamPlayer->getPlayer()->getId();

				if (const auto playerId = m_player->getPlayer()->getId(); !teamPlayer->isDead() && teamPlayerId != playerId)
				{
					teamPlayer->addHealth(m_vitalSkillTeamHealthPerTick, true);
				}
			}

		}
	}

	void onApply() override
	{
		Skill::onApply();

		m_player->setHealth(m_vitalSkillSacrificeHealth);
	}

	uint32_t getItemId() override
	{
		return SKILL_VITAL;
	}
private:
	uint32_t m_vitalSkillSacrificeHealth = 1;
	uint32_t m_vitalSkillTeamHealthPerTick = 5;
};