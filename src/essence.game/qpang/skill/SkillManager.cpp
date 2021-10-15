#include "SkillManager.h"

#include <qpang/ItemID.h>

#include <qpang/Game.h>

#include <qpang/skill/skills/ShotMoveSkill.h>
#include <qpang/skill/skills/HawkEyeSkill.h>
#include <qpang/skill/skills/CamoSkill.h>
#include <qpang/skill/skills/PocketSizeSkill.h>
#include <qpang/skill/skills/ChaosSkill.h>
#include <qpang/skill/skills/RageSkill.h>
#include <qpang/skill/skills/ShacklesSkill.h>
#include <qpang/skill/skills/VitalSkill.h>
#include <qpang/skill/skills/GiantBeast.h>

SkillManager::SkillManager()
{
}

void SkillManager::initialize()
{
	m_skills.clear();

	m_skills[GameMode::DM][ItemID::SKILL_RAPID_FIRE] = []() { return std::make_unique<RapidFireSkill>(); };
	//m_skills[GameMode::DM][ItemID::SKILL_HAWK_EYE] = []() { return std::make_unique<HawkEyeSkill>(); };
	m_skills[GameMode::DM][ItemID::SKILL_CAMO] = []() { return std::make_unique<CamoSkill>(); };
	m_skills[GameMode::DM][ItemID::SKILL_POCKET_SIZE] = []() { return std::make_unique<PocketSizeSkill>(); };
	m_skills[GameMode::DM][ItemID::SKILL_CHAOS] = []() { return std::make_unique<ChaosSkill>(); };
	m_skills[GameMode::DM][ItemID::SKILL_RAGE] = []() { return std::make_unique<RageSkill>(); };
	m_skills[GameMode::DM][ItemID::SKILL_SHACKLES] = []() { return std::make_unique<ShacklesSkill>(); };
	m_skills[GameMode::DM][ItemID::SKILL_GIANT_BEAST] = []() { return std::make_unique<GiantBeastSkill>(); };

	m_skills[GameMode::TDM][ItemID::SKILL_RAPID_FIRE] = []() { return std::make_unique<RapidFireSkill>(); };
	//m_skills[GameMode::TDM][ItemID::SKILL_HAWK_EYE] = []() { return std::make_unique<HawkEyeSkill>(); };
	m_skills[GameMode::TDM][ItemID::SKILL_CAMO] = []() { return std::make_unique<CamoSkill>(); };
	m_skills[GameMode::TDM][ItemID::SKILL_POCKET_SIZE] = []() { return std::make_unique<PocketSizeSkill>(); };
	m_skills[GameMode::TDM][ItemID::SKILL_CHAOS] = []() { return std::make_unique<ChaosSkill>(); };
	m_skills[GameMode::TDM][ItemID::SKILL_RAGE] = []() { return std::make_unique<RageSkill>(); };
	m_skills[GameMode::TDM][ItemID::SKILL_SHACKLES] = []() { return std::make_unique<ShacklesSkill>(); };
	m_skills[GameMode::TDM][ItemID::SKILL_VITAL] = []() { return std::make_unique<VitalSkill>(); };
	m_skills[GameMode::TDM][ItemID::SKILL_GIANT_BEAST] = []() { return std::make_unique<GiantBeastSkill>(); };

	m_skills[GameMode::PTE][ItemID::SKILL_RAPID_FIRE] = []() { return std::make_unique<RapidFireSkill>(); };
	//m_skills[GameMode::PTE][ItemID::SKILL_HAWK_EYE] = []() { return std::make_unique<HawkEyeSkill>(); };
	m_skills[GameMode::PTE][ItemID::SKILL_CAMO] = []() { return std::make_unique<CamoSkill>(); };
	m_skills[GameMode::PTE][ItemID::SKILL_POCKET_SIZE] = []() { return std::make_unique<PocketSizeSkill>(); };
	m_skills[GameMode::PTE][ItemID::SKILL_CHAOS] = []() { return std::make_unique<ChaosSkill>(); };
	m_skills[GameMode::PTE][ItemID::SKILL_RAGE] = []() { return std::make_unique<RageSkill>(); };
	m_skills[GameMode::PTE][ItemID::SKILL_SHACKLES] = []() { return std::make_unique<ShacklesSkill>(); };
	m_skills[GameMode::PTE][ItemID::SKILL_VITAL] = []() { return std::make_unique<VitalSkill>(); };
	m_skills[GameMode::PTE][ItemID::SKILL_GIANT_BEAST] = []() { return std::make_unique<GiantBeastSkill>(); };

	m_skills[GameMode::VIP][ItemID::SKILL_RAPID_FIRE] = []() { return std::make_unique<RapidFireSkill>(); };
	//m_skills[GameMode::VIP][ItemID::SKILL_HAWK_EYE] = []() { return std::make_unique<HawkEyeSkill>(); };
	m_skills[GameMode::VIP][ItemID::SKILL_CAMO] = []() { return std::make_unique<CamoSkill>(); };
	m_skills[GameMode::VIP][ItemID::SKILL_POCKET_SIZE] = []() { return std::make_unique<PocketSizeSkill>(); };
	m_skills[GameMode::VIP][ItemID::SKILL_CHAOS] = []() { return std::make_unique<ChaosSkill>(); };
	m_skills[GameMode::VIP][ItemID::SKILL_RAGE] = []() { return std::make_unique<RageSkill>(); };
	m_skills[GameMode::VIP][ItemID::SKILL_SHACKLES] = []() { return std::make_unique<ShacklesSkill>(); };
	m_skills[GameMode::VIP][ItemID::SKILL_VITAL] = []() { return std::make_unique<VitalSkill>(); };
	m_skills[GameMode::VIP][ItemID::SKILL_GIANT_BEAST] = []() { return std::make_unique<GiantBeastSkill>(); };

	m_skills[GameMode::PREY][ItemID::SKILL_RAPID_FIRE] = []() { return std::make_unique<RapidFireSkill>(); };
	//m_skills[GameMode::PREY][ItemID::SKILL_HAWK_EYE] = []() { return std::make_unique<HawkEyeSkill>(); };
	m_skills[GameMode::PREY][ItemID::SKILL_CAMO] = []() { return std::make_unique<CamoSkill>(); };
	m_skills[GameMode::PREY][ItemID::SKILL_POCKET_SIZE] = []() { return std::make_unique<PocketSizeSkill>(); };
	m_skills[GameMode::PREY][ItemID::SKILL_CHAOS] = []() { return std::make_unique<ChaosSkill>(); };
	m_skills[GameMode::PREY][ItemID::SKILL_RAGE] = []() { return std::make_unique<RageSkill>(); };
	m_skills[GameMode::PREY][ItemID::SKILL_SHACKLES] = []() { return std::make_unique<ShacklesSkill>(); };
	m_skills[GameMode::PREY][ItemID::SKILL_GIANT_BEAST] = []() { return std::make_unique<GiantBeastSkill>(); };
}

std::unordered_map<uint32_t, std::function<std::unique_ptr<Skill>()>> SkillManager::getSkillsForGameMode(uint8_t mode)
{
	return m_skills[mode];
}