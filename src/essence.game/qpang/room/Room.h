#pragma once

#include <memory>
#include <unordered_map>
#include <cstdint>
#include <string>
#include <mutex>

class GameMode;
class RoomPlayer;
class RoomSession;
class GameConnection;

class Room : public std::enable_shared_from_this<Room>
{
public:
	using Ptr = std::shared_ptr<Room>;

	Room(const uint32_t id, const std::u16string name, const uint8_t map, const uint8_t mode, const uint32_t host, const uint16_t port);

	void addPlayer(GameConnection* conn);
	void removePlayer(uint32_t id);
	std::shared_ptr<RoomPlayer> getPlayer(uint32_t id);
	std::shared_ptr<RoomSession> getRoomSession();

	void tick();

	void start();
	void close();
	void update(uint32_t cmd, uint32_t val);
	void finish();

	void syncPlayers(std::shared_ptr<RoomPlayer> player);
	void balancePlayers();

	bool canStartInTeam(uint8_t team) const;
	
	uint32_t getHost() const;
	uint16_t getPort() const;

	std::unordered_map<uint32_t, std::shared_ptr<RoomPlayer>> getPlayers() const;

	uint32_t getId() const;
	std::u16string getName() const;

	uint8_t getMap() const;
	void setMap(uint8_t map);

	uint8_t getMode() const;
	void setMode(uint8_t mode);

	GameMode* getModeManager() const;

	uint8_t getState() const;
	void setState(uint8_t state);

	uint8_t getPlayerCount() const;

	uint8_t getMaxPlayers() const;
	void setMaxPlayers(uint8_t maxPlayers);

	bool isLevelLimited() const;
	void setLevelLimited(bool levelLimited);

	bool isTeamSorting() const;
	void setTeamSorting(bool teamSorting);

	bool isSkillsEnabled() const;
	void setSkillsEnabled(bool skillEnabled);

	bool isMeleeOnly() const;
	void setMeleeOnly(bool meleeOnly);

	uint32_t getMasterId();
	uint32_t findNewMaster();

	uint8_t getAvailableTeam();
	bool isTeamAvailable(uint8_t team);

	uint32_t getScorePoints();
	void setScorePoints(uint32_t points);

	uint32_t getScoreTime();
	void setScoreTime(uint32_t minutes);

	bool isPointsGame();
	void setIsPointsGame(bool isPoints);

	bool isPlaying();
	void unreadyAll(bool notify = false);

	std::string getPassword();
	void setPassword(std::string& password);

	bool isEventRoom();
	void setEventRoom(bool isEventRoom = false);

	template<typename T, typename... Args>
	void broadcast(Args... args)
	{
		std::lock_guard<std::recursive_mutex> lg(m_playerMx);

		for (const auto& [id, roomPlayer] : m_players)
			roomPlayer->getConnection()->postNetEvent(new T(args...));
	}

	template<typename T, typename... Args>
	void broadcastWaiting(Args... args)
	{
		std::lock_guard<std::recursive_mutex> lg(m_playerMx);

		for (const auto& [id, roomPlayer] : m_players)
			if (!roomPlayer->isPlaying())
				roomPlayer->getConnection()->postNetEvent(new T(args...));
	}

private:

	std::recursive_mutex m_playerMx;
	std::unordered_map<uint32_t, std::shared_ptr<RoomPlayer>> m_players;

	const uint32_t m_host;
	const uint16_t m_port;

	const uint32_t m_id;
	const std::u16string m_name;

	std::string m_password;

	uint8_t m_map;
	uint8_t m_mode;
	uint8_t m_state;
	uint8_t m_maxPlayers;
	uint32_t m_masterPlayerId;
	bool m_isLevelLimited;
	bool m_isTeamSorting;
	bool m_isSkillsEnabled;
	bool m_isMeleeOnly;

	uint32_t m_scorePoints;
	uint32_t m_scoreTime;
	bool m_isPointsGame;

	bool m_isPlaying;
	bool m_isEventRoom;

	GameMode* m_modeManager;
	std::shared_ptr<RoomSession> m_roomSession;
};