#pragma once

#include <array>
#include <memory>
#include <cstdint>
#include <vector>

#include "InventoryCard.h"
#include "Skill.h"

class RoomSessionPlayer;

class PlayerSkillManager
{
public:
	void initialize(const std::shared_ptr<RoomSessionPlayer>& player);

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

	std::vector<uint32_t> getSkillTargetPlayerIds() const;
private:
	std::weak_ptr<RoomSessionPlayer> m_player;

	std::array<InventoryCard, 3> m_equippedSkillCards{};

	std::shared_ptr<Skill> m_drawnSkillCard;
	std::shared_ptr<Skill> m_activeSkillCard;

	uint32_t m_skillPoints = 0;

	uint32_t m_activeSkillCardTargetPlayerId = 0;
	uint64_t m_activeSkillCardSeqId = 0;

	double m_skillGaugeBoostPercentage = 0;

	std::vector<uint32_t> m_skillTargetPlayerIds{};
};