#pragma once

#include <cstdint>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "qpang/room/game_mode/GameModeManager.h"

class RoomManager
{
public:
	RoomManager();

	void tick();
	void remove(uint32_t id);

	std::vector<std::shared_ptr<Room>> list();
	std::shared_ptr<Room> create(const std::u16string& name, uint8_t map, uint8_t mode);
	std::shared_ptr<Room> get(uint32_t id);

	GameModeManager* getGameModeManager() const;

	std::recursive_mutex m_roomMx;
private:
	uint32_t getAvailableRoomId();

	std::unordered_map<uint32_t, std::shared_ptr<Room>> m_rooms;

	GameModeManager* m_gameModeManager;
};