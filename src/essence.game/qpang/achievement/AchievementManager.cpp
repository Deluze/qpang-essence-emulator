#include "AchievementManager.h"

#include "qpang/achievement/conditions/DeathCondition.h"
#include "qpang/achievement/conditions/KillCondition.h"
#include "qpang/achievement/conditions/StreakCondition.h"
#include "qpang/achievement/conditions/MissionLoseCondition.h"
#include "qpang/achievement/conditions/MissionWinCondition.h"
#include "qpang/achievement/conditions/GunKillCondition.h"
#include "qpang/achievement/conditions/LauncherKillCondition.h"
#include "qpang/achievement/conditions/MeleeKillCondition.h"
#include "qpang/achievement/conditions/BombKillCondition.h"
#include "qpang/achievement/conditions/HeadshotKillCondition.h"
#include "qpang/achievement/conditions/HeadshotDeathCondition.h"
#include "qpang/achievement/conditions/TeamKillCondition.h"
#include "qpang/achievement/conditions/TeamDeathCondition.h"
#include "qpang/achievement/conditions/TeamWinCondition.h"
#include "qpang/achievement/conditions/TeamLoseCondition.h"
#include "qpang/achievement/conditions/MultiKillCondition.h"

#include "qpang/room/session/player/RoomSessionPlayer.h"
#include "qpang/player/Player.h"
#include "qpang/player/achievement/AchievementContainer.h"

AchievementManager::AchievementManager()
{
	m_achievements[AchievementID::WARRIOR] = new KillCondition(1000);
	m_achievements[AchievementID::FIGHTER] = new KillCondition(5000);
	m_achievements[AchievementID::SLAYER] = new KillCondition(10000);
	m_achievements[AchievementID::CHAMPION] = new KillCondition(20000);
	m_achievements[AchievementID::DESTROY_KING] = new KillCondition(50000);

	m_achievements[AchievementID::FRIEND] = new DeathCondition(1000);
	m_achievements[AchievementID::LOVER] = new DeathCondition(5000);
	m_achievements[AchievementID::ADDICT] = new DeathCondition(10000);
	m_achievements[AchievementID::STOCKER] = new DeathCondition(20000);
	m_achievements[AchievementID::MADNESS] = new DeathCondition(50000);

	m_achievements[AchievementID::CHIEF] = new TeamWinCondition(100);
	m_achievements[AchievementID::HEAD] = new TeamWinCondition(200);
	m_achievements[AchievementID::LEADER] = new TeamWinCondition(500);
	m_achievements[AchievementID::BOSS] = new TeamWinCondition(1000);
	m_achievements[AchievementID::HERO] = new TeamWinCondition(10000);

	m_achievements[AchievementID::TUMBLER] = new TeamLoseCondition(100);
	m_achievements[AchievementID::FAITHFUL] = new TeamLoseCondition(200);
	m_achievements[AchievementID::SINGLE_MINDED] = new TeamLoseCondition(500);
	m_achievements[AchievementID::IRON_WILL] = new TeamLoseCondition(1000);
	m_achievements[AchievementID::SUPERMAN] = new TeamLoseCondition(10000);

	m_achievements[AchievementID::TRICKY] = new MissionWinCondition(100);
	m_achievements[AchievementID::BRAIN] = new MissionWinCondition(200);
	m_achievements[AchievementID::ANALYST] = new MissionWinCondition(500);
	m_achievements[AchievementID::STAFF] = new MissionWinCondition(1000);
	m_achievements[AchievementID::STRATEGIST] = new MissionWinCondition(10000);

	m_achievements[AchievementID::NOVICE] = new MissionLoseCondition(100);
	m_achievements[AchievementID::HARD_WORKING] = new MissionLoseCondition(200);
	m_achievements[AchievementID::CHALLENGER] = new MissionLoseCondition(500);
	m_achievements[AchievementID::TRANSCENDENT] = new MissionLoseCondition(1000);
	m_achievements[AchievementID::ART_SEEKER] = new MissionLoseCondition(10000);

	m_achievements[AchievementID::SHOOTING_STAR] = new GunKillCondition(1000);
	m_achievements[AchievementID::DEMOLITIONER] = new LauncherKillCondition(1000);
	m_achievements[AchievementID::PITCHER] = new BombKillCondition(1000);
	m_achievements[AchievementID::SWORD_MASTER] = new MeleeKillCondition(1000);

	m_achievements[AchievementID::HEAD_COLLECTOR] = new HeadshotKillCondition(5000);
	m_achievements[AchievementID::BIG_HEAD] = new HeadshotDeathCondition(1000);

	m_achievements[AchievementID::MANAGER] = new StreakCondition(10);
	m_achievements[AchievementID::HEAD_CHIEF] = new StreakCondition(20);
	m_achievements[AchievementID::PRESIDENT] = new StreakCondition(30);
	m_achievements[AchievementID::CHAIRMAN] = new StreakCondition(40);
	m_achievements[AchievementID::GODS_SON] = new StreakCondition(50);

	m_achievements[AchievementID::LUCKY_HAND] = new MultiKillCondition(2);
	m_achievements[AchievementID::BRASS_HAND] = new MultiKillCondition(3);
	m_achievements[AchievementID::SILVER_HAND] = new MultiKillCondition(5);
	m_achievements[AchievementID::GOLDEN_HAND] = new MultiKillCondition(10);
	m_achievements[AchievementID::GODS_HAND] = new MultiKillCondition(15);
	
	m_achievements[AchievementID::SPY] = new TeamKillCondition(100);
	m_achievements[AchievementID::SUSPECT] = new TeamDeathCondition(100);
}

void AchievementManager::onPlayerFinish(const std::shared_ptr<RoomSessionPlayer>& session)
{
	auto player = session->getPlayer();
	auto* achievementContainer = player->getAchievementContainer();

	for (const auto& [id, achievement] : m_achievements)
		if (achievement->matches(session))
			achievementContainer->unlock(static_cast<uint32_t>(id));
}
