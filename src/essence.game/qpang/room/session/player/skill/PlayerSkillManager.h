#pragma once

#include <array>
#include <memory>
#include <cstdint>

#include "Skill.h"

class RoomSessionPlayer;

class PlayerSkillManager
{
public:
	void initialize(std::shared_ptr<RoomSessionPlayer> player);

	void tick();

	void activateSkillCard(uint32_t targetPlayerId, uint32_t seqId);
	void deactivateSkillCard();

	void failSkillCard(uint32_t targetPlayerId, uint32_t seqId);

	void updateSkillPointsForPlayer();

	void addSkillPoints(uint32_t skillPoints);
	void removeSkillPoints(uint32_t skillPoints);
	void resetSkillPoints();

	uint32_t drawSkill();

	std::shared_ptr<Skill> getActiveSkillCard();
	std::shared_ptr<Skill> getDrawnSkillCard();

	uint32_t getActiveSkillCardTargetPlayerId();
	uint32_t getActiveSkillCardSeqId();

	uint32_t getRequiredSkillPoints();

	bool hasSufficientSkillPoints();

	bool hasActiveSkillCard();
	bool isDrawnSkillCard(uint32_t itemId);
private:
	std::weak_ptr<RoomSessionPlayer> m_player;

	std::shared_ptr<Skill> m_drawnSkillCard;
	std::shared_ptr<Skill> m_activeSkillCard;

	uint32_t m_skillPoints;

	uint32_t m_activeSkillCardTargetPlayerId;
	uint32_t m_activeSkillCardSeqId;
};