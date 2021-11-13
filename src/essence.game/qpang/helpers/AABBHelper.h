#pragma once

#include "Position.h"
#include "Spawn.h"

// ReSharper disable once CppInconsistentNaming
class AABBHelper
{
public:
	static bool isPositionInBetweenBounds(const Position point, const Position minBound, const Position maxBound);
	static bool IsPositionWithinSpawnRadius(Position position, Spawn spawn, float radius);
private:
	static bool isNegative(const float value);
	static bool isInRange(const float value, const float min, const float max);
	static bool isInRangeNegative(const float value, const float min, const float max);
};
