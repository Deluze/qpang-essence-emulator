#pragma once

#include <cstdint>

class RandomHelper
{
public:
	static uint32_t getRandomNumber(uint32_t min, uint32_t max);
	static float getRandomFloat(float min, float max);
};
