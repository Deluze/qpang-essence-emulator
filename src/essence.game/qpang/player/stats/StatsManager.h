#pragma once

#include <memory>
#include <cstdint>

class Player;
class RoomSessionPlayer;

class StatsManager
{
public:
	void initialize(std::shared_ptr<Player> player, uint32_t playerId);
	
	void save();

	uint32_t getKills();
	uint32_t getDeaths();
	uint32_t getNormalWins();
	uint32_t getNormalLosses();
	uint32_t getNormalDrews();
	uint32_t getMissionWins();
	uint32_t getMissionLosses();
	uint32_t getMissionDrews();

	uint32_t getPlayTime();
	uint32_t getPlayTimeInMinutes();

	uint32_t getSlackerPoints();

	uint32_t getMeleeKills();
	uint32_t getGunKills();
	uint32_t getLauncherKills();
	uint32_t getBombKills();

	uint32_t getHeadshotKills();
	uint32_t getHeadshotDeaths();

	uint32_t getTeamKills();
	uint32_t getTeamDeaths();

	void apply(std::shared_ptr<RoomSessionPlayer> player);

	void addKills(uint32_t val);
	void addDeaths(uint32_t val);
	void clearKD();
	void addPlaytime(uint32_t val);
	
	void addNormalWin();
	void addNormalLoss();
	void addNormalDraw();
	void addMissionWin();
	void addMissionLoss();
	void addMissionDraw();
	void clearWL();
	void addSlackerPoint();

	void addMeleeKills(uint32_t val = 1);
	void addGunKills(uint32_t val = 1);
	void addLauncherKills(uint32_t val = 1);
	void addBombKills(uint32_t val = 1);

	void addHeadshotKills(uint32_t val = 1);
	void addHeadshotDeaths(uint32_t val = 1);

	void addTeamKills(uint32_t val = 1);
	void addTeamDeaths(uint32_t val = 1);
	
private:
	std::weak_ptr<Player> m_player;

	uint32_t m_kills = 0;
	uint32_t m_deaths = 0;
	uint32_t m_normalWon = 0;
	uint32_t m_normalLost = 0;
	uint32_t m_normalDrew = 0;
	uint32_t m_missionWon = 0;
	uint32_t m_missionLost = 0;
	uint32_t m_missionDrew = 0;
	uint32_t m_slackerPoints = 0;
	uint32_t m_playTime = 0;

	uint32_t m_meleeKills = 0;
	uint32_t m_gunKills = 0;
	uint32_t m_launcherKills = 0;
	uint32_t m_bombKills = 0;

	uint32_t m_headshotKills = 0;
	uint32_t m_headshotDeaths = 0;

	uint32_t m_teamKills = 0;
	uint32_t m_teamDeaths = 0;

	uint32_t m_eventItemPickUps = 0;
};