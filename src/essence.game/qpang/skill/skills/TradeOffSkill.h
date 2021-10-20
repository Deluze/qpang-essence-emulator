#pragma once

class TradeOffSkill final : public Skill
{
public:
	TradeOffSkill() : Skill(false, 0)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_TRADE_OFF;
	}

	void onApply() override
	{
		m_player->getSkillManager()->resetSkillPoints();

		const auto myTeam = m_player->getTeam();
		const auto enemyTeam = static_cast<uint8_t>(myTeam == 1 ? 2 : 1);

		const auto roomSession = m_player->getRoomSession();

		if (roomSession == nullptr)
		{
			return;
		}

		const auto enemyPlayers = roomSession->getPlayersForTeam(enemyTeam);

		for (const auto &enemyPlayer : enemyPlayers)
		{
			enemyPlayer->getSkillManager()->removeSkillPoints(100);
			enemyPlayer->getPlayer()->broadcast(u"Your skillpoints have been reduced by 1 due to the Trade Off skill.");
		}

		m_player->getSkillManager()->deactivateSkillCard();
	}

	uint32_t getSkillPointCost() override
	{
		return 3;
	}

	SkillRateType getSkillRateType() override
	{
		return SkillRateType::GOLD;
	}

	SkillTargetType getSkillTarget() override
	{
		return SkillTargetType::ENEMY_TEAM;
	}
};