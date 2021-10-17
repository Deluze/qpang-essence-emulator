#include "SkillManager.h"

#include <qpang/ItemID.h>
#include <qpang/Game.h>

#include <qpang/skill/skills/RapidFireSkill.h>
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
#include <qpang/skill/skills/WeaponSealSkill.h>
#include <qpang/skill/skills/SkillSealSkill.h>
#include <qpang/skill/skills/MentalUpSkill.h>

SkillManager::SkillManager()
{
}

void SkillManager::initialize()
{
	m_skills.clear();

	// TODO: Rework this some day cause this is god awful code
	initializeDeathMatchSkills();
	initializeTeamDeathMatchSkills();
	initializeProtectTheEssenceSkills();
	initializeVipSkills();
}

std::vector<std::function<std::unique_ptr<Skill>()>> SkillManager::getSkillsForGameMode(uint8_t mode)
{
	return m_skills[mode];
}

void SkillManager::initializeDeathMatchSkills()
{
	for (uint8_t i = 0; i < 4; i++)
	{
		m_skills[GameMode::DM].push_back([]() { return std::make_unique<ChaosSkill>(); });
		m_skills[GameMode::DM].push_back([]() { return std::make_unique<CamoSkill>(); });
		m_skills[GameMode::DM].push_back([]() { return std::make_unique<RageSkill>(); });
		m_skills[GameMode::DM].push_back([]() { return std::make_unique<RapidFireSkill>(); });
		m_skills[GameMode::DM].push_back([]() { return std::make_unique<ShacklesSkill>(); });
	}

	for (uint8_t i = 0; i < 3; i++)
	{
		m_skills[GameMode::DM].push_back([]() { return std::make_unique<AssassinSkill>(); });
		m_skills[GameMode::DM].push_back([]() { return std::make_unique<MentalUpSkill>(); });
		m_skills[GameMode::DM].push_back([]() { return std::make_unique<PocketSizeSkill>(); });
		m_skills[GameMode::DM].push_back([]() { return std::make_unique<ReflectSkill>(); });
		m_skills[GameMode::DM].push_back([]() { return std::make_unique<SkillSealSkill>(); });
		m_skills[GameMode::DM].push_back([]() { return std::make_unique<WeaponSealSkill>(); });
	}

	for (uint8_t i = 0; i < 1; i++)
	{
		m_skills[GameMode::DM].push_back([]() { return std::make_unique<TranseSkill>(); });
		m_skills[GameMode::DM].push_back([]() { return std::make_unique<GiantBeastSkill>(); });
		m_skills[GameMode::DM].push_back([]() { return std::make_unique<LastWeaponSkill>(); });
	}
}

void SkillManager::initializeTeamDeathMatchSkills() 
{
	for (uint8_t i = 0; i < 4; i++)
	{
		m_skills[GameMode::TDM].push_back([]() { return std::make_unique<ChaosSkill>(); });
		m_skills[GameMode::TDM].push_back([]() { return std::make_unique<CamoSkill>(); });
		m_skills[GameMode::TDM].push_back([]() { return std::make_unique<RageSkill>(); });
		m_skills[GameMode::TDM].push_back([]() { return std::make_unique<RapidFireSkill>(); });
		m_skills[GameMode::TDM].push_back([]() { return std::make_unique<ShacklesSkill>(); });
		m_skills[GameMode::TDM].push_back([]() { return std::make_unique<TrapSkill>(); });
	}

	for (uint8_t i = 0; i < 3; i++)
	{
		m_skills[GameMode::TDM].push_back([]() { return std::make_unique<AssassinSkill>(); });
		m_skills[GameMode::TDM].push_back([]() { return std::make_unique<MentalUpSkill>(); });
		m_skills[GameMode::TDM].push_back([]() { return std::make_unique<PocketSizeSkill>(); });
		m_skills[GameMode::TDM].push_back([]() { return std::make_unique<ReflectSkill>(); });
		m_skills[GameMode::TDM].push_back([]() { return std::make_unique<SkillSealSkill>(); });
		m_skills[GameMode::TDM].push_back([]() { return std::make_unique<WeaponSealSkill>(); });
	}

	for (uint8_t i = 0; i < 2; i++)
	{
		m_skills[GameMode::TDM].push_back([]() { return std::make_unique<VitalSkill>(); });
	}

	for (uint8_t i = 0; i < 1; i++)
	{
		m_skills[GameMode::TDM].push_back([]() { return std::make_unique<TranseSkill>(); });
		m_skills[GameMode::TDM].push_back([]() { return std::make_unique<GiantBeastSkill>(); });
		m_skills[GameMode::TDM].push_back([]() { return std::make_unique<LastWeaponSkill>(); });
	}
}

void SkillManager::initializeProtectTheEssenceSkills()
{
	for (uint8_t i = 0; i < 4; i++)
	{
		m_skills[GameMode::PTE].push_back([]() { return std::make_unique<ChaosSkill>(); });
		m_skills[GameMode::PTE].push_back([]() { return std::make_unique<CamoSkill>(); });
		m_skills[GameMode::PTE].push_back([]() { return std::make_unique<RageSkill>(); });
		m_skills[GameMode::PTE].push_back([]() { return std::make_unique<RapidFireSkill>(); });
		m_skills[GameMode::PTE].push_back([]() { return std::make_unique<ShacklesSkill>(); });
		m_skills[GameMode::PTE].push_back([]() { return std::make_unique<TrapSkill>(); });
	}

	for (uint8_t i = 0; i < 3; i++)
	{
		m_skills[GameMode::PTE].push_back([]() { return std::make_unique<AssassinSkill>(); });
		m_skills[GameMode::PTE].push_back([]() { return std::make_unique<MentalUpSkill>(); });
		m_skills[GameMode::PTE].push_back([]() { return std::make_unique<PocketSizeSkill>(); });
		m_skills[GameMode::PTE].push_back([]() { return std::make_unique<ReflectSkill>(); });
		m_skills[GameMode::PTE].push_back([]() { return std::make_unique<SkillSealSkill>(); });
		m_skills[GameMode::PTE].push_back([]() { return std::make_unique<WeaponSealSkill>(); });
	}

	for (uint8_t i = 0; i < 2; i++)
	{
		m_skills[GameMode::PTE].push_back([]() { return std::make_unique<VitalSkill>(); });
	}

	for (uint8_t i = 0; i < 1; i++)
	{
		m_skills[GameMode::PTE].push_back([]() { return std::make_unique<TranseSkill>(); });
		m_skills[GameMode::PTE].push_back([]() { return std::make_unique<GiantBeastSkill>(); });
		m_skills[GameMode::PTE].push_back([]() { return std::make_unique<LastWeaponSkill>(); });
	}
}

void SkillManager::initializeVipSkills() 
{
	for (uint8_t i = 0; i < 4; i++)
	{
		m_skills[GameMode::VIP].push_back([]() { return std::make_unique<ChaosSkill>(); });
		m_skills[GameMode::VIP].push_back([]() { return std::make_unique<CamoSkill>(); });
		m_skills[GameMode::VIP].push_back([]() { return std::make_unique<RageSkill>(); });
		m_skills[GameMode::VIP].push_back([]() { return std::make_unique<RapidFireSkill>(); });
		m_skills[GameMode::VIP].push_back([]() { return std::make_unique<ShacklesSkill>(); });
		m_skills[GameMode::VIP].push_back([]() { return std::make_unique<TrapSkill>(); });
	}

	for (uint8_t i = 0; i < 3; i++)
	{
		m_skills[GameMode::VIP].push_back([]() { return std::make_unique<AssassinSkill>(); });
		m_skills[GameMode::VIP].push_back([]() { return std::make_unique<MentalUpSkill>(); });
		m_skills[GameMode::VIP].push_back([]() { return std::make_unique<PocketSizeSkill>(); });
		m_skills[GameMode::VIP].push_back([]() { return std::make_unique<ReflectSkill>(); });
		m_skills[GameMode::VIP].push_back([]() { return std::make_unique<SkillSealSkill>(); });
		m_skills[GameMode::VIP].push_back([]() { return std::make_unique<WeaponSealSkill>(); });
	}

	for (uint8_t i = 0; i < 2; i++)
	{
		m_skills[GameMode::VIP].push_back([]() { return std::make_unique<VitalSkill>(); });
	}

	for (uint8_t i = 0; i < 1; i++)
	{
		m_skills[GameMode::VIP].push_back([]() { return std::make_unique<TranseSkill>(); });
		m_skills[GameMode::VIP].push_back([]() { return std::make_unique<GiantBeastSkill>(); });
		m_skills[GameMode::VIP].push_back([]() { return std::make_unique<LastWeaponSkill>(); });
	}
}
