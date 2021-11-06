#pragma once

#include <memory>
#include <cstdint>
#include <vector>

#include "Skill.h"

class RoomSessionPlayer;

class PlayerSkillManager
{
public:
	void initialize(const std::shared_ptr<RoomSessionPlayer>& player);
	void setupEquippedSkillCards();
	void initializeSkillGaugeBoosters();

	void tick();

	void activateSkillCard(const std::shared_ptr<Skill>& skill, uint32_t targetPlayerId, uint64_t seqId, uint32_t cardType);
	void deactivateSkillCard();

	void failSkillCard(const std::shared_ptr<Skill>& skill, uint32_t targetPlayerId, uint64_t seqId, uint32_t cardType);

	bool isDrawnOrEquippedSkillCard(uint32_t itemId) const;

	void updateSkillPointsForPlayer() const;

	void addSkillPoints(uint32_t skillPoints);
	void removeSkillPoints(uint32_t skillPoints);
	void resetSkillPoints();

	uint32_t drawSkill();

	std::shared_ptr<Skill> getActiveSkillCard() const;
	std::shared_ptr<Skill> getDrawnSkillCard() const;

	std::shared_ptr<Skill> getEquippedSkillCard(uint64_t seqId) const;
	std::array<std::shared_ptr<Skill>, 3> getEquippedSkillCards() const;

	uint32_t getActiveSkillCardTargetPlayerId() const;
	uint64_t getActiveSkillCardSeqId() const;

	static uint32_t getRequiredSkillPoints(const std::shared_ptr<Skill>& skill);

	double getSkillGaugeBoostPercentage() const;

	bool hasSufficientSkillPoints(const std::shared_ptr<Skill>& skill) const;

	bool hasActiveSkillCard() const;
	bool isDrawnSkillCard(uint32_t itemId) const;

	std::vector<uint32_t> getSkillTargetPlayerIds() const;

private:
	std::weak_ptr<RoomSessionPlayer> m_player;

	std::shared_ptr<Skill> m_firstSkillCard;
	std::shared_ptr<Skill> m_secondSkillCard;
	std::shared_ptr<Skill> m_thirdSkillCard;

	std::shared_ptr<Skill> m_drawnSkillCard;
	std::shared_ptr<Skill> m_activeSkillCard;

	uint32_t m_cardType = 0;

	uint32_t m_skillPoints = 0;

	uint32_t m_activeSkillCardTargetPlayerId = 0;
	uint64_t m_activeSkillCardSeqId = 0;

	double m_skillGaugeBoostPercentage = 0;

	std::vector<uint32_t> m_skillTargetPlayerIds{};
};