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
#include <qpang/skill/skills/LastWeaponSkill.h>
#include <qpang/skill/skills/TranseSkill.h>
#include <qpang/skill/skills/TrapSkill.h>
#include <qpang/skill/skills/IronWallSkill.h>
#include <qpang/skill/skills/ReflectSkill.h>
#include <qpang/skill/skills/AssassinSkill.h>

SkillManager::SkillManager()
{
}

void SkillManager::initialize()
{
	m_skills.clear();

	m_skills[GameMode::DM][ItemID::SKILL_RAPID_FIRE] = []() { return std::make_unique<RapidFireSkill>(); };
	m_skills[GameMode::DM][ItemID::SKILL_CAMO] = []() { return std::make_unique<CamoSkill>(); };
	m_skills[GameMode::DM][ItemID::SKILL_POCKET_SIZE] = []() { return std::make_unique<PocketSizeSkill>(); };
	m_skills[GameMode::DM][ItemID::SKILL_CHAOS] = []() { return std::make_unique<ChaosSkill>(); };
	m_skills[GameMode::DM][ItemID::SKILL_RAGE] = []() { return std::make_unique<RageSkill>(); };
	m_skills[GameMode::DM][ItemID::SKILL_SHACKLES] = []() { return std::make_unique<ShacklesSkill>(); };
	m_skills[GameMode::DM][ItemID::SKILL_GIANT_BEAST] = []() { return std::make_unique<GiantBeastSkill>(); };
	m_skills[GameMode::DM][ItemID::SKILL_TRANSE] = []() { return std::make_unique<TranseSkill>(); };
	m_skills[GameMode::DM][ItemID::SKILL_LAST_WEAPON] = []() { return std::make_unique<LastWeaponSkill>(); };
	//m_skills[GameMode::DM][ItemID::SKILL_IRON_WALL] = []() { return std::make_unique<IronWallSkill>(); };
	m_skills[GameMode::DM][ItemID::SKILL_REFLECT] = []() { return std::make_unique<ReflectSkill>(); };
	m_skills[GameMode::DM][ItemID::SKILL_ASSASSIN] = []() { return std::make_unique<AssassinSkill>(); };

	m_skills[GameMode::TDM][ItemID::SKILL_RAPID_FIRE] = []() { return std::make_unique<RapidFireSkill>(); };
	m_skills[GameMode::TDM][ItemID::SKILL_CAMO] = []() { return std::make_unique<CamoSkill>(); };
	m_skills[GameMode::TDM][ItemID::SKILL_POCKET_SIZE] = []() { return std::make_unique<PocketSizeSkill>(); };
	m_skills[GameMode::TDM][ItemID::SKILL_CHAOS] = []() { return std::make_unique<ChaosSkill>(); };
	m_skills[GameMode::TDM][ItemID::SKILL_RAGE] = []() { return std::make_unique<RageSkill>(); };
	m_skills[GameMode::TDM][ItemID::SKILL_SHACKLES] = []() { return std::make_unique<ShacklesSkill>(); };
	m_skills[GameMode::TDM][ItemID::SKILL_VITAL] = []() { return std::make_unique<VitalSkill>(); };
	m_skills[GameMode::TDM][ItemID::SKILL_GIANT_BEAST] = []() { return std::make_unique<GiantBeastSkill>(); };
	m_skills[GameMode::TDM][ItemID::SKILL_TRANSE] = []() { return std::make_unique<TranseSkill>(); };
	m_skills[GameMode::TDM][ItemID::SKILL_LAST_WEAPON] = []() { return std::make_unique<LastWeaponSkill>(); };
	m_skills[GameMode::TDM][ItemID::SKILL_TRAP] = []() { return std::make_unique<TrapSkill>(); };
	//m_skills[GameMode::TDM][ItemID::SKILL_IRON_WALL] = []() { return std::make_unique<IronWallSkill>(); };
	m_skills[GameMode::TDM][ItemID::SKILL_REFLECT] = []() { return std::make_unique<ReflectSkill>(); };
	m_skills[GameMode::TDM][ItemID::SKILL_ASSASSIN] = []() { return std::make_unique<AssassinSkill>(); };

	m_skills[GameMode::PTE][ItemID::SKILL_RAPID_FIRE] = []() { return std::make_unique<RapidFireSkill>(); };
	m_skills[GameMode::PTE][ItemID::SKILL_CAMO] = []() { return std::make_unique<CamoSkill>(); };
	m_skills[GameMode::PTE][ItemID::SKILL_POCKET_SIZE] = []() { return std::make_unique<PocketSizeSkill>(); };
	m_skills[GameMode::PTE][ItemID::SKILL_CHAOS] = []() { return std::make_unique<ChaosSkill>(); };
	m_skills[GameMode::PTE][ItemID::SKILL_RAGE] = []() { return std::make_unique<RageSkill>(); };
	m_skills[GameMode::PTE][ItemID::SKILL_SHACKLES] = []() { return std::make_unique<ShacklesSkill>(); };
	m_skills[GameMode::PTE][ItemID::SKILL_VITAL] = []() { return std::make_unique<VitalSkill>(); };
	m_skills[GameMode::PTE][ItemID::SKILL_GIANT_BEAST] = []() { return std::make_unique<GiantBeastSkill>(); };
	m_skills[GameMode::PTE][ItemID::SKILL_TRANSE] = []() { return std::make_unique<TranseSkill>(); };
	m_skills[GameMode::PTE][ItemID::SKILL_LAST_WEAPON] = []() { return std::make_unique<LastWeaponSkill>(); };
	m_skills[GameMode::PTE][ItemID::SKILL_TRAP] = []() { return std::make_unique<TrapSkill>(); };
	//m_skills[GameMode::PTE][ItemID::SKILL_IRON_WALL] = []() { return std::make_unique<IronWallSkill>(); };
	m_skills[GameMode::PTE][ItemID::SKILL_REFLECT] = []() { return std::make_unique<ReflectSkill>(); };
	m_skills[GameMode::PTE][ItemID::SKILL_ASSASSIN] = []() { return std::make_unique<AssassinSkill>(); };

	m_skills[GameMode::VIP][ItemID::SKILL_RAPID_FIRE] = []() { return std::make_unique<RapidFireSkill>(); };
	m_skills[GameMode::VIP][ItemID::SKILL_CAMO] = []() { return std::make_unique<CamoSkill>(); };
	m_skills[GameMode::VIP][ItemID::SKILL_POCKET_SIZE] = []() { return std::make_unique<PocketSizeSkill>(); };
	m_skills[GameMode::VIP][ItemID::SKILL_CHAOS] = []() { return std::make_unique<ChaosSkill>(); };
	m_skills[GameMode::VIP][ItemID::SKILL_RAGE] = []() { return std::make_unique<RageSkill>(); };
	m_skills[GameMode::VIP][ItemID::SKILL_SHACKLES] = []() { return std::make_unique<ShacklesSkill>(); };
	m_skills[GameMode::VIP][ItemID::SKILL_VITAL] = []() { return std::make_unique<VitalSkill>(); };
	m_skills[GameMode::VIP][ItemID::SKILL_GIANT_BEAST] = []() { return std::make_unique<GiantBeastSkill>(); };
	m_skills[GameMode::VIP][ItemID::SKILL_TRANSE] = []() { return std::make_unique<TranseSkill>(); };
	m_skills[GameMode::VIP][ItemID::SKILL_LAST_WEAPON] = []() { return std::make_unique<LastWeaponSkill>(); };
	m_skills[GameMode::VIP][ItemID::SKILL_TRAP] = []() { return std::make_unique<TrapSkill>(); };
	//m_skills[GameMode::VIP][ItemID::SKILL_IRON_WALL] = []() { return std::make_unique<IronWallSkill>(); };
	m_skills[GameMode::VIP][ItemID::SKILL_REFLECT] = []() { return std::make_unique<ReflectSkill>(); };
	m_skills[GameMode::VIP][ItemID::SKILL_ASSASSIN] = []() { return std::make_unique<AssassinSkill>(); };
}

std::unordered_map<uint32_t, std::function<std::unique_ptr<Skill>()>> SkillManager::getSkillsForGameMode(uint8_t mode)
{
	return m_skills[mode];
}