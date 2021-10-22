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

	void activateSkillCard(uint32_t targetPlayerId, uint64_t seqId);
	void deactivateSkillCard();

	void failSkillCard(uint32_t targetPlayerId, uint64_t seqId);

	void updateSkillPointsForPlayer() const;

	void addSkillPoints(uint32_t skillPoints);
	void removeSkillPoints(uint32_t skillPoints);
	void resetSkillPoints();

	uint32_t drawSkill();

	std::shared_ptr<Skill> getActiveSkillCard() const;
	std::shared_ptr<Skill> getDrawnSkillCard() const;

	uint32_t getActiveSkillCardTargetPlayerId() const;
	uint64_t getActiveSkillCardSeqId() const;

	uint32_t getRequiredSkillPoints() const;

	double getSkillGaugeBoostPercentage() const;

	bool hasSufficientSkillPoints() const;

	bool hasActiveSkillCard() const;
	bool isDrawnSkillCard(uint32_t itemId) const;
private:
	std::weak_ptr<RoomSessionPlayer> m_player;

	std::shared_ptr<Skill> m_drawnSkillCard;
	std::shared_ptr<Skill> m_activeSkillCard;

	uint32_t m_skillPoints;

	uint32_t m_activeSkillCardTargetPlayerId;
	uint64_t m_activeSkillCardSeqId;

	double m_skillGaugeBoostPercentage;
};