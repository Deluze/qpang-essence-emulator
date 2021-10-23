#include "SkillManager.h"

#include <qpang/Game.h>

#include "AssassinSkill.h"
#include "BlessingSkill.h"
#include "CamoSkill.h"
#include "ChaosSkill.h"
#include "GiantBeast.h"
#include "IronWallSkill.h"
#include "LastWeaponSkill.h"
#include "MentalUpSkill.h"
#include "PocketSizeSkill.h"
#include "RageSkill.h"
#include "RapidFireSkill.h"
#include "ReflectSkill.h"
#include "ReplaySkill.h"
#include "ShacklesSkill.h"
#include "SkillSealSkill.h"
#include "StuntUpSkill.h"
#include "TeamCheerSkill.h"
#include "TradeOffSkill.h"
#include "TranseSkill.h"
#include "TrapSkill.h"
#include "UndercoverSkill.h"
#include "VitalSkill.h"
#include "WeaponSealSkill.h"

class IronWallSkill;
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
	for (size_t i = 0; i < 4; i++)
	{
		if (i < 4)
		{
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<ChaosSkill>(); });
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<CamoSkill>(); });
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<IronWallSkill>(); });
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<RageSkill>(); });
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<RapidFireSkill>(); });
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<ReplaySkill>(); });
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<ShacklesSkill>(); });
		}

		if (i < 3)
		{
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<AssassinSkill>(); });
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<MentalUpSkill>(); });
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<PocketSizeSkill>(); });
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<ReflectSkill>(); });
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<SkillSealSkill>(); });
			m_skills[GameMode::DM].push_back([]() { return std::make_unique<StuntUpSkill>(); });
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
	for (size_t i = 0; i < 4; i++)
	{
		if (i < 4)
		{
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<ChaosSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<CamoSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<IronWallSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<RageSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<RapidFireSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<ReplaySkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<ShacklesSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<TrapSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<UndercoverSkill>(); });
		}

		if (i < 3)
		{
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<AssassinSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<MentalUpSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<PocketSizeSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<ReflectSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<SkillSealSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<StuntUpSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<TeamCheerSkill>(); });
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<WeaponSealSkill>(); });
		}

		if (i < 2)
		{
			m_skills[GameMode::TDM].push_back([]() { return std::make_unique<VitalSkill>(); });
			//m_skills[GameMode::TDM].push_back([]() { return std::make_unique<BlessingSkill>(); });
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
