#pragma once

class VitalSkill final : public Skill
{
	static constexpr uint16_t SACRIFICE_HEALTH_VALUE = 1;
	static constexpr uint16_t TEAM_EXTRA_HEALTH_PER_TICK = 5;

public:
	VitalSkill() : Skill(true, 10)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_VITAL;
	}

	void tick() override
	{
		Skill::tick();

		if (const auto roomSession = m_player->getRoomSession(); roomSession != nullptr)
		{
			const auto teamPlayers = roomSession->getPlayersForTeam(m_player->getTeam());

			// Iterate over all team players that are alive and give them 5 health each tick for the entire duration of the skillcard.
			for (const auto& teamPlayer : teamPlayers)
			{
				const auto teamPlayerId = teamPlayer->getPlayer()->getId();

				if (const auto playerId = m_player->getPlayer()->getId(); !teamPlayer->isDead() && teamPlayerId != playerId)
				{
					teamPlayer->addHealth(TEAM_EXTRA_HEALTH_PER_TICK, true);
				}
			}

		}
	}

	void onApply() override
	{
		Skill::onApply();

		m_player->setHealth(SACRIFICE_HEALTH_VALUE, true);
	}

	uint32_t getSkillPointCost() override
	{
		return 3;
	}

	SkillRateType getSkillRateType() override
	{
		return SkillRateType::GOLD;
	}

	SkillTargetType getSkillTargetType() override
	{
		return SkillTargetType::ALLY_TEAM;
	}
};