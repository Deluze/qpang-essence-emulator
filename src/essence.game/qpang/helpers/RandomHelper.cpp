#include "RandomHelper.h"

#include <random>
#include <map>


uint32_t RandomHelper::getRandomNumber(const uint32_t from, const uint32_t to)
{
	std::random_device rd;
	std::map<int, int> hist;

	// Note: See https://stackoverflow.com/questions/50662280/c-need-a-good-technique-for-seeding-rand-that-does-not-use-time
	// ReSharper disable once CppRedundantCastExpression
	const std::uniform_int_distribution dist(from, to);

	return dist(rd);
}
