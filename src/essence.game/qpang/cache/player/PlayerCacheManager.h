#pragma once

#include <unordered_map>
#include <mutex>
#include <cstdint>
#include <string>

#include "qpang/player/Player.h"

class PlayerCacheManager
{
public:
	Player::Ptr tryGet(uint32_t playerId);
	Player::Ptr tryGet(const std::u16string& nickname);

	Player::Ptr cache(uint32_t playerId);
	Player::Ptr cache(const std::u16string& nickname);
	void cache(Player::Ptr player);

	void invalidate(uint32_t playerId);

	void clear();
private:
	std::mutex m_cacheMx;

	std::unordered_map<uint32_t, Player::Ptr> m_playersById;
	std::unordered_map<std::u16string, Player::Ptr> m_playersByName;
};