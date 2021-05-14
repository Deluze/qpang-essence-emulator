#pragma once

#include <memory>
#include <vector>
#include <cstdint>

class Player;
class Room;
class RoomSessionPlayer;
class GameConnection;
class GameNetEvent;

class RoomPlayer
{
public:
	using Ptr = std::shared_ptr<RoomPlayer>;

	RoomPlayer(GameConnection* conn, std::shared_ptr<Room> room);
	~RoomPlayer();

	std::shared_ptr<Player> getPlayer();
	std::shared_ptr<Room> getRoom();

	// When calling this MAKE SURE that the lifetime of RoomPlayer is longer than of the GameConnection this function returns
	GameConnection* getConnection();

	void send(GameNetEvent* evt);

	uint8_t getTeam();
	void setTeam(uint8_t team);

	bool isReady();
	void setReady(bool isReady = true);

	bool isPlaying();
	void setPlaying(bool isPlaying = true);

	bool isSpectating();
	void setSpectating(bool isSpectating = true);

	void setRoomSessionPlayer(std::shared_ptr<RoomSessionPlayer> player);
	
	void onStart();

	std::shared_ptr<RoomSessionPlayer> getRoomSessionPlayer();
private:
	uint8_t m_team;
	bool m_isReady;
	bool m_isPlaying;
	bool m_isSpectating;

	GameConnection* m_conn = nullptr;
	std::shared_ptr<Room> m_room;
	std::weak_ptr<RoomSessionPlayer> m_roomSessionPlayer;


};