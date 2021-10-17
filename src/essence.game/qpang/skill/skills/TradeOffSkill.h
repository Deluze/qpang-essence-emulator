#pragma once

#include <iostream>

class TradeOffSkill : public Skill
{
public:
	TradeOffSkill() : Skill()
	{
		m_requiredSkillPoints = 3;

		m_hasDuration = false;
		m_durationInSeconds = 0;

		m_skillTarget = SkillTarget::ALL_ENEMY_PLAYERS;
		m_isReflectableSkillCard = false;
	}

	uint32_t getItemId() override
	{
		return ItemID::SKILL_TRADE_OFF;
	}

	void onApply() override
	{
		m_player->getSkillManager()->resetSkillPoints();

		const auto myTeam = m_player->getTeam();
		const auto enemyTeam = (myTeam == 1) ? 2 : 1;

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
private:
};