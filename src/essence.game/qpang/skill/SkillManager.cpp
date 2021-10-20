#include "SkillManager.h"

#include <qpang/Game.h>

#include <qpang/skill/skills/RapidFireSkill.h>
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
#include <qpang/skill/skills/ReflectSkill.h>
#include <qpang/skill/skills/AssassinSkill.h>
#include <qpang/skill/skills/WeaponSealSkill.h>
#include <qpang/skill/skills/SkillSealSkill.h>
#include <qpang/skill/skills/MentalUpSkill.h>
#include <qpang/skill/skills/TradeOffSkill.h>

SkillManager::SkillManager()
= default;

void SkillManager::initialize()
{
	m_skills.clear();

	initializeDeathMatchSkills();
	initializeTeamDeathMatchSkills();
}

std::vector<std::function<std::unique_ptr<Skill>()>> SkillManager::getSkillsForGameMode(uint8_t mode)
{
	if (mode == GameMode::PTE || mode == GameMode::VIP)
	{
		mode = GameMode::TDM;
	}

	return m_skills[mode];
}

void SkillManager::initializeDeathMatchSkills()
{
	for (uint8_t i = 0; i < 4; i++)
	{
		if (i < 4)
		{
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<ChaosSkill>(); });
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<CamoSkill>(); });
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<RageSkill>(); });
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<RapidFireSkill>(); });
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<ShacklesSkill>(); });
		}

		if (i < 3)
		{
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<AssassinSkill>(); });
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<MentalUpSkill>(); });
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<PocketSizeSkill>(); });
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<ReflectSkill>(); });
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<SkillSealSkill>(); });
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<WeaponSealSkill>(); });
		}

		if (i < 1)
		{
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<TranseSkill>(); });
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<GiantBeastSkill>(); });
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<LastWeaponSkill>(); });
		}
	}
}

void SkillManager::initializeTeamDeathMatchSkills() 
{
	for (uint8_t i = 0; i < 4; i++)
	{
		if (i < 4)
		{
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<ChaosSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<CamoSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<RageSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<RapidFireSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<ShacklesSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<TrapSkill>(); });
		}

		if (i < 3)
		{
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<AssassinSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<MentalUpSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<PocketSizeSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<ReflectSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<SkillSealSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<WeaponSealSkill>(); });
		}

		if (i < 2)
		{
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<VitalSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<TradeOffSkill>(); });
		}

		if (i < 1)
		{
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<TranseSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<GiantBeastSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<LastWeaponSkill>(); });
		}
	}
}
