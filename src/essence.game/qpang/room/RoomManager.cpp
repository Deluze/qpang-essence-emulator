#include "RoomManager.h"

#include "core/Emulator.h"
#include "core/config/ConfigManager.h"

#include "qpang/Game.h"
#include "qpang/room/Room.h"

RoomManager::RoomManager()
{
	m_gameModeManager = new GameModeManager();
}

void RoomManager::tick()
{
	std::lock_guard<std::recursive_mutex> lg(m_roomMx);

	for (const auto& [id, room] : m_rooms)
		room->tick();
}

std::vector<std::shared_ptr<Room>> RoomManager::list()
{
	std::lock_guard<std::recursive_mutex> lg(m_roomMx);

	std::vector<std::shared_ptr<Room>> rooms;

	for (const auto& [id, room] : m_rooms)
		rooms.push_back(room);

	return rooms;
}

std::shared_ptr<Room> RoomManager::create(std::u16string name, uint8_t map, uint8_t mode)
{
	uint32_t host = inet_addr(CONFIG_MANAGER->getString("HOST").c_str());
	uint16_t port = static_cast<uint16_t>(CONFIG_MANAGER->getInt("GAME_PORT"));

	auto id = getAvailableRoomId();
	auto room = std::make_shared<Room>(id, name, map, mode, host, port);

	m_roomMx.lock();
	m_rooms[id] = room;
	m_roomMx.unlock();
	
	return room;
}

void RoomManager::remove(uint32_t id)
{
	std::lock_guard<std::recursive_mutex> lg(m_roomMx);

	m_rooms.erase(id);
}

Room::Ptr RoomManager::get(uint32_t id)
{
	std::lock_guard<std::recursive_mutex> lg(m_roomMx);

	auto it = m_rooms.find(id);

	if (it == m_rooms.cend())
		return nullptr;

	return it->second;
}

GameModeManager* RoomManager::getGameModeManager()
{
	return m_gameModeManager;
}

uint32_t RoomManager::getAvailableRoomId()
{
	std::lock_guard<std::recursive_mutex> lg(m_roomMx);

	uint32_t id{ 1 };

	while (true)
	{
		const auto it = m_rooms.find(id);

		if (it == m_rooms.cend())
			break;

		id++;
	}

	return id;
}
