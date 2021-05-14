#include "StatsManager.h"

#include <cmath>

#include "core/Emulator.h"
#include "core/database/Database.h"

#include "qpang/Game.h"
#include "qpang/player/Player.h"
#include "qpang/room/session/RoomSession.h"
#include "qpang/room/session/player/RoomSessionPlayer.h"

void StatsManager::initialize(std::shared_ptr<Player> player, uint32_t playerId)
{
	m_player = player;

	auto stmt = DATABASE->prepare("SELECT * FROM player_stats WHERE player_id = ?");
	stmt->bindInteger(playerId);
	auto res = stmt->fetch();

	if (!res->hasResults())
		return; // ?

	m_kills = res->getInt("kills");
	m_deaths = res->getInt("deaths");
	m_normalWon = res->getInt("n_won");
	m_normalLost = res->getInt("n_lost");
	m_normalDrew = res->getInt("n_drew");
	m_missionWon = res->getInt("m_won");
	m_missionLost = res->getInt("m_lost");
	m_missionDrew = res->getInt("m_drew");
	m_playTime = res->getInt("playtime");
	m_slackerPoints = res->getInt("slacker_points");
	m_meleeKills = res->getInt("melee_kills");
	m_gunKills = res->getInt("gun_kills");
	m_launcherKills = res->getInt("launcher_kills");
	m_bombKills = res->getInt("bomb_kills");
	m_headshotKills = res->getInt("headshot_kills");
	m_headshotDeaths = res->getInt("headshot_deaths");
	m_teamKills = res->getInt("team_kills");
	m_teamDeaths = res->getInt("team_deaths");
	m_eventItemPickUps = res->getInt("event_item_pickups");
}

void StatsManager::save()
{
	if (const auto player = m_player.lock(); player != nullptr)
	{
		DATABASE_DISPATCHER->dispatch(
			"UPDATE player_stats " \
			"SET kills = ?, deaths = ?, n_won = ?, n_lost = ?, n_drew = ?, m_won = ?, m_lost = ?, m_drew = ?, playtime = ?, slacker_points = ?, " \
			"melee_kills = ?, gun_kills = ?, launcher_kills = ?, bomb_kills = ?, headshot_kills = ?, headshot_deaths = ?, team_kills = ?, team_deaths = ?, event_item_pickups = ? "
			"WHERE player_id = ?",
			{
				m_kills,
				m_deaths,
				m_normalWon,
				m_normalLost,
				m_normalDrew,
				m_missionWon,
				m_missionLost,
				m_missionDrew,
				m_playTime,
				m_slackerPoints,
				m_meleeKills,
				m_gunKills,
				m_launcherKills,
				m_bombKills,
				m_headshotKills,
				m_headshotDeaths,
				m_teamKills,
				m_teamDeaths,
				m_eventItemPickUps,
				player->getId()
			}
		);
	}
}

uint32_t StatsManager::getKills()
{
	return m_kills;
}

uint32_t StatsManager::getDeaths()
{
	return m_deaths;
}

uint32_t StatsManager::getNormalWins()
{
	return m_normalWon;
}

uint32_t StatsManager::getNormalLosses()
{
	return m_normalLost;
}

uint32_t StatsManager::getNormalDrews()
{
	return m_normalDrew;
}

uint32_t StatsManager::getMissionWins()
{
	return m_missionWon;
}

uint32_t StatsManager::getMissionLosses()
{
	return m_missionLost;
}

uint32_t StatsManager::getMissionDrews()
{
	return m_missionDrew;
}

uint32_t StatsManager::getPlayTime()
{
	return m_playTime;
}

uint32_t StatsManager::getPlayTimeInMinutes()
{
	return m_playTime / 60;
}

uint32_t StatsManager::getSlackerPoints()
{
	return m_slackerPoints;
}

uint32_t StatsManager::getHeadshotKills()
{
	return m_headshotKills;
}

uint32_t StatsManager::getHeadshotDeaths()
{
	return m_headshotDeaths;
}

uint32_t StatsManager::getTeamKills()
{
	return m_teamKills;
}

uint32_t StatsManager::getTeamDeaths()
{
	return m_teamDeaths;
}

uint32_t StatsManager::getMeleeKills()
{
	return m_meleeKills;
}

uint32_t StatsManager::getGunKills()
{
	return m_gunKills;
}

uint32_t StatsManager::getLauncherKills()
{
	return m_launcherKills;
}

uint32_t StatsManager::getBombKills()
{
	return m_bombKills;
}

void StatsManager::apply(std::shared_ptr<RoomSessionPlayer> player)
{
	addKills(player->getKills());
	addDeaths(player->getDeaths());
	addPlaytime(player->getPlaytime());
	m_eventItemPickUps += player->getEventItemPickUps();

	save();
}

void StatsManager::addKills(uint32_t val)
{
	m_kills += val;
}

void StatsManager::addDeaths(uint32_t val)
{
	m_deaths += val;
}

void StatsManager::clearKD()
{
	m_kills = 0;
	m_deaths = 0;
	m_teamKills = 0;
	m_teamDeaths = 0;
	m_meleeKills = 0;
	m_gunKills = 0;
	m_launcherKills = 0;
	m_bombKills = 0;
	m_headshotKills = 0;
	m_headshotDeaths = 0;

	save();
}

void StatsManager::addPlaytime(uint32_t val)
{
	m_playTime += val;
}

void StatsManager::addNormalWin()
{
	m_normalWon++;
}

void StatsManager::addNormalLoss()
{
	m_normalLost++;
}

void StatsManager::addNormalDraw()
{
	m_normalDrew++;
}

void StatsManager::addMissionWin()
{
	m_missionWon++;
}

void StatsManager::addMissionLoss()
{
	m_missionLost++;
}

void StatsManager::addMissionDraw()
{
	m_missionDrew++;
}

void StatsManager::clearWL()
{
	m_normalWon = 0;
	m_normalLost = 0;
	m_normalDrew = 0;
	m_missionWon = 0;
	m_missionLost = 0;
	m_missionDrew = 0;

	save();
}

void StatsManager::addSlackerPoint()
{
	m_slackerPoints++;
}

void StatsManager::addMeleeKills(uint32_t val)
{
	m_meleeKills += val;
}

void StatsManager::addGunKills(uint32_t val)
{
	m_gunKills += val;
}

void StatsManager::addLauncherKills(uint32_t val)
{
	m_launcherKills += val;
}

void StatsManager::addBombKills(uint32_t val)
{
	m_bombKills += val;
}

void StatsManager::addHeadshotKills(uint32_t val)
{
	m_headshotKills += val;
}

void StatsManager::addHeadshotDeaths(uint32_t val)
{
	m_headshotDeaths += val;
}

void StatsManager::addTeamKills(uint32_t val)
{
	m_teamKills += val;
}

void StatsManager::addTeamDeaths(uint32_t val)
{
	m_teamDeaths += val;
}
