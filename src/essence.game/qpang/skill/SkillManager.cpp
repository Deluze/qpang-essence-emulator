#include "SkillManager.h"

#include <qpang/skill/skills/ShotMoveSkill.h>

SkillManager::SkillManager()
{
}

void SkillManager::initialize()
{
	m_skills.clear();

	m_skills[GameMode::DM][SkillID::SHOT_MOVE] = std::make_shared<ShotMoveSkill>();
	m_skills[GameMode::TDM][SkillID::SHOT_MOVE] = std::make_shared<ShotMoveSkill>();
	m_skills[GameMode::PTE][SkillID::SHOT_MOVE] = std::make_shared<ShotMoveSkill>();
	m_skills[GameMode::VIP][SkillID::SHOT_MOVE] = std::make_shared<ShotMoveSkill>();
	m_skills[GameMode::PREY][SkillID::SHOT_MOVE] = std::make_shared<ShotMoveSkill>();
}

std::unordered_map<uint32_t, std::shared_ptr<Skill>> SkillManager::getSkillsForGameMode(uint8_t mode)
{
	return m_skills[mode];
}