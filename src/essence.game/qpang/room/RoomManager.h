#pragma once

#include <unordered_map>
#include <vector>
#include <cstdint>
#include <mutex>
#include <memory>

#include "qpang/room/game_mode/GameModeManager.h"

class RoomManager
{
public:
	RoomManager();

	void tick();
	void remove(uint32_t id);

	std::vector<std::shared_ptr<Room>> list();
	std::shared_ptr<Room> create(std::u16string name, uint8_t map, uint8_t mode);
	std::shared_ptr<Room> get(uint32_t id);

	GameModeManager* getGameModeManager();
private:
	uint32_t getAvailableRoomId();

	std::recursive_mutex m_roomMx;
	std::unordered_map<uint32_t, std::shared_ptr<Room>> m_rooms;

	GameModeManager* m_gameModeManager;
};