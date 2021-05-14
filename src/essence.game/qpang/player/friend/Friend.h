#pragma once

#include <cstdint>
#include <string>

struct Friend
{
	uint32_t playerId;
	std::u16string nickname;
	uint8_t level;
	uint8_t rank;
	uint8_t state;
	bool isOnline;
};