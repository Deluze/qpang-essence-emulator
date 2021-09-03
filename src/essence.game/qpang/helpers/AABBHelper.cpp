#include "AABBHelper.h"
#include "Position.h"

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