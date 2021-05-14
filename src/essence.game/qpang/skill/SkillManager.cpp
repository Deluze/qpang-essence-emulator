#include "SkillManager.h"

#include "qpang/skill/skills/Assassin.h"
#include "qpang/skill/skills/Camo.h"
#include "qpang/skill/skills/Absorb.h"
#include "qpang/skill/skills/TeamCheer.h"
#include "qpang/skill/skills/Zilla.h"

#include "qpang/ItemID.h"

#include "qpang/room/game_mode/GameMode.h"

SkillManager::SkillManager()
{
}

void SkillManager::initialize()
{
	auto& dm = m_skills[GameMode::DM];

	dm[ItemID::SKILL_CAMO] = []() { return std::make_unique<Camo>(); };
	dm[ItemID::SKILL_ASSASSIN] = []() { return std::make_unique<Assassin>(); };
	dm[ItemID::SKILL_ABSORB] = []() { return std::make_unique<Absorb>(); };
	dm[ItemID::SKILL_TEAM_CHEER] = []() { return std::make_unique<TeamCheer>(); };
	dm[ItemID::SKILL_ZILLA] = []() { return std::make_unique<Zilla>(); };
}

std::unordered_map<uint32_t, std::function<std::unique_ptr<Skill>()>> SkillManager::getSkillsForMode(uint8_t mode)
{
	return m_skills[mode];
}
