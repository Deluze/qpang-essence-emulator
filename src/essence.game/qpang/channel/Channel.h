#pragma once

#include <cstdint>
#include <string>

struct Channel
{
	uint32_t id;
	std::u16string name;
	uint8_t minLevel;
	uint8_t maxLevel;
	uint16_t maxPlayers;
	uint16_t currPlayers;
};