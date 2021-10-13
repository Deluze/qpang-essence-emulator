#include "SkillManager.h"

#include <qpang/skill/skills/ShotMoveSkill.h>
#include <qpang/ItemID.h>

SkillManager::SkillManager()
{
}

void SkillManager::initialize()
{
	m_skills.clear();

	m_skills[GameMode::DM][ItemID::SKILL_SHOT_MOVE] = []() { return std::make_unique<ShotMoveSkill>(); };
	m_skills[GameMode::TDM][ItemID::SKILL_SHOT_MOVE] = []() { return std::make_unique<ShotMoveSkill>(); };
	m_skills[GameMode::PTE][ItemID::SKILL_SHOT_MOVE] = []() { return std::make_unique<ShotMoveSkill>(); };
	m_skills[GameMode::VIP][ItemID::SKILL_SHOT_MOVE] = []() { return std::make_unique<ShotMoveSkill>(); };
	m_skills[GameMode::PREY][ItemID::SKILL_SHOT_MOVE] = []() { return std::make_unique<ShotMoveSkill>(); };
}

std::unordered_map<uint32_t, std::function<std::unique_ptr<Skill>()>> SkillManager::getSkillsForGameMode(uint8_t mode)
{
	return m_skills[mode];
}