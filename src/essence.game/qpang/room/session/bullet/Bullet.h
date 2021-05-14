#pragma once

#include <cstdint>
#include <array>
#include <mutex>

class Bullet
{
public:
	void addHit(uint32_t playerId)
	{
		hits[hitIndex] = playerId;

		if (hitIndex < 15)
			hitIndex++;
	}

	bool hasHit(uint32_t playerId)
	{
		return std::find(hits.cbegin(), hits.cend(), playerId) != hits.cend();
	}

	uint32_t id = 0;
	uint32_t itemId = 0;
	uint8_t killCount = 0;
	uint8_t hitIndex = 0;
	time_t shootTime = time(NULL);
	std::array<uint32_t, 16> hits;
};