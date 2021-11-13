#include "AABBHelper.h"
#include "Position.h"
#include "Spawn.h"

// ReSharper disable once CppInconsistentNaming
bool AABBHelper::isPositionInBetweenBounds(const Position point, const Position minBound, const Position maxBound)
{
	const auto xPositionInBetweenBounds = (isNegative(point.x)
		? isInRangeNegative(point.x, minBound.x, maxBound.x)
		: isInRange(point.x, minBound.x, maxBound.x));

	const auto yPositionInBetweenBounds = (isNegative(point.y)
		? isInRangeNegative(point.y, minBound.y, maxBound.y)
		: isInRange(point.y, minBound.y, maxBound.y));

	const auto zPositionInBetweenBounds = (isNegative(point.z)
		? isInRangeNegative(point.z, minBound.z, maxBound.z)
		: isInRange(point.z, minBound.z, maxBound.z));

	return (xPositionInBetweenBounds && yPositionInBetweenBounds && zPositionInBetweenBounds);
}

bool AABBHelper::IsPositionWithinSpawnRadius(const Position position, const Spawn spawn, const float radius)
{
	const Spawn spawnMin{ spawn.x - radius, spawn.y - radius, spawn.z - radius };
	const Spawn spawnMax{ spawn.x + radius, spawn.y + radius, spawn.z + radius };

	const auto xIsInRange = (position.x >= spawnMin.x && position.x <= spawnMax.x);
	const auto zIsInRange = (position.z >= spawnMin.z && position.z <= spawnMax.z);

	return (xIsInRange && zIsInRange);
}

bool AABBHelper::isNegative(const float value)
{
	return (value < 0);
}

bool AABBHelper::isInRange(const float value, const float min, const float max)
{
	return (value >= min && value <= max);
}

bool AABBHelper::isInRangeNegative(const float value, const float min, const float max)
{
	return (value <= min && value >= max);
}