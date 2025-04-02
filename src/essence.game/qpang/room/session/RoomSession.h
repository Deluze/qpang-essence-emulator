#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <mutex>

#include "ConfigManager.h"
#include "RoomSessionBossFightManager.h"
#include "RoomSessionNpcManager.h"
#include "RoomSessionObjectManager.h"
#include "RoomSessionPveAreaManager.h"
#include "RoomSessionPveItemManager.h"
#include "RoomSessionPveRoundManager.h"
#include "RoomSessionPveWaveManager.h"

#include "qpang/room/session/player/RoomSessionPlayer.h"
#include "qpang/room/session/game_item/GameItemManager.h"
#include "qpang/room/session/skill/RoomSessionSkillManager.h"
#include "qpang/room/session/pathfinding/Pathfinder.h"

class Room;
class GameMode;

class RoomSession : public std::enable_shared_from_this<RoomSession>
{
public:
	using Ptr = std::shared_ptr<RoomSession>;

	RoomSession(std::shared_ptr<Room> room, GameMode* mode);

	void initialize();

	void addPlayer(GameConnection* conn, uint8_t team);
	bool removePlayer(uint32_t playerId);
	RoomSessionPlayer::Ptr find(uint32_t playerId);
	RoomSessionPlayer::Ptr findEligibleVip(uint8_t team, bool noConditions);

	void handlePlayerFinish(RoomSessionPlayer::Ptr player);
	void handlePlayerPveFinish(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer);

	void tick();
	void clear();

	bool isFinished() const;
	void finish();
	bool canFinish();
	bool isAlmostFinished();

	// Note: Pve finish.
	void finishPveGame(bool didWin = true);
	static bool canFinishPveGame();

	void setLastRespawnLocation(Spawn spawn);
	Spawn getLastRespawnLocation() const;

	void addPointsForTeam(uint8_t team, uint32_t amount = 1);
	void addBluePoints(uint32_t amount = 1);
	void addYellowPoints(uint32_t amount = 1);

	uint32_t getBluePoints();
	uint32_t getYellowPoints();

	void spawnPlayer(RoomSessionPlayer::Ptr player);
	void syncPlayer(RoomSessionPlayer::Ptr player);

	uint32_t getElapsedTime() const; // in milliseconds
	time_t getEndTime() const;
	uint32_t getTimeLeftInSeconds() const;

	void killPlayer(RoomSessionPlayer::Ptr killer, RoomSessionPlayer::Ptr target, uint32_t weaponId, bool isHeadshot);

	GameMode* getGameMode();
	GameItemManager* getItemManager();
	RoomSessionSkillManager* getSkillManager();

	RoomSessionNpcManager* getNpcManager();
	RoomSessionObjectManager* getObjectManager();
	RoomSessionPveItemManager* getPveItemManager();
	RoomSessionPveRoundManager* getPveRoundManager();
	RoomSessionPveAreaManager* getPveAreaManager();
	RoomSessionPveWaveManager* getPveWaveManager();
	RoomSessionBossFightManager* getBossFightManager();

	Pathfinder* getAboveGroundPathfinder();
	Pathfinder* getUnderGroundPathfinder();

	std::shared_ptr<Room> getRoom();

	std::vector<RoomSessionPlayer::Ptr> getPlayers();
	std::vector<RoomSessionPlayer::Ptr> getPlayingPlayers();
	std::vector<RoomSessionPlayer::Ptr> getAlivePlayingPlayersExcept(const uint32_t playerId, const uint8_t team);
	std::vector<RoomSessionPlayer::Ptr> RoomSession::getEligiblePlayersToBeSelectedAsTag();
	std::vector<RoomSessionPlayer::Ptr> getPlayersForTeam(uint8_t team);
	uint32_t getPointsForTeam(uint8_t team);
	uint32_t getTopScore();

	void resetTime();
	void stopTime();

	bool areAllPlayersPermanentlyDead();

#pragma region Essence

	void resetEssence();
	void setEssenceHolder(RoomSessionPlayer::Ptr player);
	void setEssencePosition(const Spawn& spawn);
	Spawn getEssencePosition();
	RoomSessionPlayer::Ptr getEssenceHolder();
	bool isEssenceDropped();
	bool isEssenceReset();
	uint32_t getElapsedEssenceDropTime();

#pragma endregion

#pragma region Vip

	void findNextBlueVip();
	bool isNextBlueVipEligible();
	void setBlueVip(RoomSessionPlayer::Ptr player);
	uint32_t getElapsedBlueVipTime();
	RoomSessionPlayer::Ptr getBlueVip();

	void findNextYellowVip();
	bool isNextYellowVipEligible();
	void setYellowVip(RoomSessionPlayer::Ptr player);
	uint32_t getElapsedYellowVipTime();
	RoomSessionPlayer::Ptr getYellowVip();

	bool isVip(RoomSessionPlayer::Ptr player);

#pragma endregion

#pragma region PublicEnemy

	bool attemptToFindNextTag();

	void resetCurrentlySelectedTag();
	void selectNextTag(uint32_t playerId);

	void broadcastNextTagHasBeenSelected();

	bool isTagSelected();
	uint32_t getCurrentlySelectedTag();

	uint32_t getTagCountdown();

	bool hasTagCountdownEnded();
	void initiateTagCountdown(uint32_t countdownTime = 5000);
	void decreaseTagCountdown(uint32_t countdownTime = 1000);

	uint32_t getInitialWaitTime();

	bool hasInitialWaitTimeElapsed();
	void decreaseInitialWaitTime(uint32_t time = 1000);
	void clearInitialWaitTime();

	void initiateSearchForNextTag();
	bool isSearchingForNextTag();
	void stopSearchingForNextTag();

#pragma endregion

	template<typename T, typename... Args>
	void relay(Args... args)
	{
		std::lock_guard<std::recursive_mutex> lg(m_playerMx);

		for (const auto& [id, session] : m_players)
			session->post(new T(args...));
	};

	template<typename T, typename... Args>
	void relayExcept(uint32_t playerId, Args... args)
	{
		std::lock_guard<std::recursive_mutex> lg(m_playerMx);

		for (const auto& [id, session] : m_players)
			if (id != playerId)
			{
				T* evt = new T(args...);
				session->post(evt);
			}
	};

	template<typename T, typename... Args>
	void relayPlaying(Args... args)
	{
		std::lock_guard<std::recursive_mutex> lg(m_playerMx);

		for (const auto& [id, session] : m_players)
			if (session->isPlaying())
			{
				T* evt = new T(args...);
				session->post(evt);
			}
	};

	template<typename T, typename... Args>
	void relayPlayingExcept(uint32_t playerId, Args... args)
	{
		std::lock_guard<std::recursive_mutex> lg(m_playerMx);

		for (const auto& [id, session] : m_players)
			if (id != playerId)
				if (session->isPlaying())
					session->post(new T(args...));
	};

	template<typename... Args>
	void relayState(Args... args)
	{
		std::lock_guard<std::recursive_mutex> lg(m_playerMx);

		for (const auto& [id, session] : m_players)
			session->post(new GCGameState(id, args...));
	};

private:
	std::shared_ptr<Room> m_room;

	bool m_isFinished;
	bool m_isPoints;

	uint32_t m_goal;
	uint32_t m_bluePoints;
	uint32_t m_yellowPoints;

	time_t m_startTime;
	time_t m_endTime;
	time_t m_lastTickTime;

	time_t m_essenceDropTime;
	bool m_isEssenceReset;

	GameMode* m_gameMode;
	GameItemManager m_itemManager;
	RoomSessionSkillManager m_skillManager;

	RoomSessionNpcManager m_npcManager;
	RoomSessionObjectManager m_objectManager;
	RoomSessionPveItemManager m_pveItemManager;
	RoomSessionPveRoundManager m_pveRoundManager;
	RoomSessionPveAreaManager m_pveAreaManager;
	RoomSessionPveWaveManager m_pveWaveManager;
	RoomSessionBossFightManager m_roomSessionBossFightManager;

	Pathfinder m_aboveGroundPathfinder;
	Pathfinder m_underGroundPathfinder;

	std::recursive_mutex m_playerMx;
	std::unordered_map<uint32_t, RoomSessionPlayer::Ptr> m_players;

	std::recursive_mutex m_leaverMx;
	std::vector<RoomSessionPlayer::Ptr> m_leavers;

	RoomSessionPlayer::Ptr m_essenceHolder;
	Spawn m_essencePosition = {
		0,
		0,
		0,
	};

	RoomSessionPlayer::Ptr m_blueVip;
	RoomSessionPlayer::Ptr m_nextBlueVip;
	time_t m_blueVipSetTime;
	RoomSessionPlayer::Ptr m_yellowVip;
	RoomSessionPlayer::Ptr m_nextYellowVip;
	time_t m_yellowVipSetTime;

	uint32_t m_currentlySelectedTag;

	bool m_isSearchingForNextTag;

	uint32_t m_tagCountdown;
	uint32_t m_initialWaitTime;

	Spawn m_lastRespawnLocation{};

	std::vector<uint32_t> m_previouslySelectedTagPlayers = {};
};