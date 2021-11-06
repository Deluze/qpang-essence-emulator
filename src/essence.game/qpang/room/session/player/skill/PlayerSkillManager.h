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

	void activateSkill(const std::shared_ptr<Skill>& skill, uint32_t targetPlayerId, uint64_t seqId, uint32_t cardType);
	void deactivateSkill();

	void failSkill(const std::shared_ptr<Skill>& skill, uint32_t targetPlayerId, uint64_t seqId, uint32_t cardType);

	bool isDrawnOrEquippedSkill(uint32_t itemId) const;

	void updateSkillPointsForPlayer() const;

	void addSkillPoints(uint32_t skillPoints);
	void removeSkillPoints(uint32_t skillPoints);
	void resetSkillPoints();

	uint32_t drawSkill();

	std::shared_ptr<Skill> getActiveSkill() const;
	std::shared_ptr<Skill> getDrawnSkill() const;

	std::shared_ptr<Skill> getEquippedSkill(uint64_t seqId) const;
	std::array<std::shared_ptr<Skill>, 3> getEquippedSkills() const;

	uint32_t getActiveSkillTargetPlayerId() const;
	uint64_t getActiveSkillSeqId() const;
	uint32_t getActiveSkillCardType();

	static uint32_t getRequiredSkillPoints(const std::shared_ptr<Skill>& skill);

	double getSkillGaugeBoostPercentage() const;

	bool hasSufficientSkillPoints(const std::shared_ptr<Skill>& skill) const;

	bool hasActiveSkill() const;
	bool isDrawnSkill(uint32_t itemId) const;

	std::vector<uint32_t> getSkillTargetPlayerIds() const;

private:
	std::weak_ptr<RoomSessionPlayer> m_player;

	std::shared_ptr<Skill> m_firstSkill;
	std::shared_ptr<Skill> m_secondSkill;
	std::shared_ptr<Skill> m_thirdSkill;

	std::shared_ptr<Skill> m_drawnSkill;
	std::shared_ptr<Skill> m_activeSkill;

	uint32_t m_activeSkillCardType = 0;

	uint32_t m_skillPoints = 0;

	uint32_t m_activeSkillTargetPlayerId = 0;
	uint64_t m_activeSkillSeqId = 0;

	double m_skillGaugeBoostPercentage = 0;

	std::vector<uint32_t> m_skillTargetPlayerIds{};
};