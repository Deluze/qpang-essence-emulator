#pragma once

#include <memory>
#include <cstdint>

class Player;
class Room;
class RoomSessionPlayer;
class GameConnection;
class GameNetEvent;

class RoomPlayer  // NOLINT(cppcoreguidelines-special-member-functions)
{
public:
	// ReSharper disable once CppInconsistentNaming
	using Ptr = std::shared_ptr<RoomPlayer>;

	RoomPlayer(GameConnection* conn, std::shared_ptr<Room> room);
	~RoomPlayer();

	[[nodiscard]] std::shared_ptr<Player> getPlayer() const;
	[[nodiscard]] std::shared_ptr<Room> getRoom() const;

	// When calling this MAKE SURE that the lifetime of RoomPlayer is longer than of the GameConnection this function returns
	[[nodiscard]] GameConnection* getConnection() const;

	void send(GameNetEvent* evt) const;

	[[nodiscard]] uint8_t getTeam() const;
	void setTeam(uint8_t team);

	[[nodiscard]] bool isReady() const;
	void setReady(bool isReady = true);

	[[nodiscard]] bool isPlaying() const;
	void setPlaying(bool isPlaying = true);

	[[nodiscard]] bool isSpectating() const;
	void setSpectating(bool isSpectating = true);

	void setRoomSessionPlayer(std::shared_ptr<RoomSessionPlayer> player);
	
	void onStart() const;

	[[nodiscard]] std::shared_ptr<RoomSessionPlayer> getRoomSessionPlayer() const;
private:
	uint8_t m_team;
	bool m_isReady;
	bool m_isPlaying;
	bool m_isSpectating;

	GameConnection* m_conn = nullptr;
	std::shared_ptr<Room> m_room;
	std::weak_ptr<RoomSessionPlayer> m_roomSessionPlayer;
};
