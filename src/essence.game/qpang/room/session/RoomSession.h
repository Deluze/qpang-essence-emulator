#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <array>

#include "qpang/room/session/player/RoomSessionPlayer.h"
#include "qpang/room/session/game_item/GameItemManager.h"
#include "qpang/room/session/skill/RoomSkillManager.h"

constexpr auto PUBLIC_ENEMY_INITIAL_WAIT_TIME = 6000;

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

	void tick();
	void clear();

	bool isFinished();
	void finish();
	bool canFinish();
	bool isAlmostFinished();

	void addPointsForTeam(uint8_t team, uint32_t amount = 1);
	void addBluePoints(uint32_t amount = 1);
	void addYellowPoints(uint32_t amount = 1);

	uint32_t getBluePoints();
	uint32_t getYellowPoints();

	void spawnPlayer(RoomSessionPlayer::Ptr player);
	void syncPlayer(RoomSessionPlayer::Ptr player);

	uint32_t getElapsedTime(); // in milliseconds
	uint32_t getTimeLeftInSeconds();

	void killPlayer(RoomSessionPlayer::Ptr killer, RoomSessionPlayer::Ptr target, uint32_t weaponId, bool isHeadshot);

	GameMode* getGameMode();
	GameItemManager* getItemManager();
	RoomSkillManager* getSkillManager();
	std::shared_ptr<Room> getRoom();

	std::vector<RoomSessionPlayer::Ptr> getPlayers();
	std::vector<RoomSessionPlayer::Ptr> getPlayingPlayers();
	std::vector<RoomSessionPlayer::Ptr> RoomSession::getEligibleTagPlayers();
	std::vector<RoomSessionPlayer::Ptr> getPlayersForTeam(uint8_t team);
	uint32_t getPointsForTeam(uint8_t team);
	uint32_t getTopScore();

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

	void findNextTag();

	void setIsNextTagTransforming(bool isTransforming);
	bool getIsNextTagTransforming();

	void clearCurrentTag();
	void selectNextTag(uint32_t playerId);

	void broadcastNextTagHasBeenSelected();

	uint32_t getLastSelectedTag();
	uint32_t getCurrentlySelectedTag();

	uint32_t getSelectTagCountdownTime();

	void setSelectTagCountdownTime(uint32_t countdownTime);
	void decreaseSelectTagCountdownTime(uint32_t countdownTime);
	
	void decreaseSelectTagInitialWaitTime(uint32_t waitTime);
	void clearSelectTagInitialWaitTime();
	uint32_t getSelectTagInitialWaitTime();

	bool isFindingNextTag();
	void setIsFindingNextTag(bool isFindingNextTag);

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
	RoomSkillManager m_skillManager;

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
	uint32_t m_lastSelectedTag;

	bool m_isNextTagTransforming;
	bool m_isFindingNextTag;

	uint32_t m_selectTagCountdownTime;
	uint32_t m_selectTagInitialWaitTime = PUBLIC_ENEMY_INITIAL_WAIT_TIME;
};