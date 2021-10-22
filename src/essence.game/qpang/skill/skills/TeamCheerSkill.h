#pragma once

#include "gc_card.hpp"
#include "Skill.h"

class TeamCheerSkill final : public Skill
{
public:
	TeamCheerSkill() : Skill(true, 15)
	{
	}

	uint32_t getItemId() override
	{
		return SKILL_TEAM_CHEER;
	}

	void onApply() override
	{
		if (const auto roomSession = m_player->getRoomSession(); roomSession != nullptr)
		{
			const auto srcPlayerId = m_player->getPlayer()->getId();

			const auto itemId = getItemId();
			const auto activeSkillCardSeqId = m_player->getSkillManager()->getActiveSkillCardSeqId();

			const auto teamPlayers = roomSession->getPlayersForTeam(m_player->getTeam());

			// NOTE: Only the player that executes the card (m_player), receives the "no stamina loss" effect.
			// TODO: Make sure every team player receives this effect as well, this is not the case right now.
		}
	}

	uint32_t getSkillPointCost() override
	{
		return 2;
	}

	SkillRateType getSkillRateType() override
	{
		return SkillRateType::SILVER;
	}

	SkillTargetType getSkillTargetType() override
	{
		return SkillTargetType::ALLY_TEAM;
	}
};
