#include "RandomHelper.h"

#include <map>
#include <random>

#include <time.h>

uint32_t RandomHelper::getRandomNumber(const uint32_t min, const uint32_t max)
{
	std::random_device rd;
	std::map<int, int> hist;

	// Note: See https://stackoverflow.com/questions/50662280/c-need-a-good-technique-for-seeding-rand-that-does-not-use-time
	// ReSharper disable once CppRedundantCastExpression
	const std::uniform_int_distribution dist(min, max);

	return dist(rd);
}

float RandomHelper::getRandomFloat(const float min, const float max)
{
	std::default_random_engine e(static_cast <unsigned> (time(nullptr)));

	const std::uniform_real_distribution<> dis(min, max);

	return dis(e);
}
