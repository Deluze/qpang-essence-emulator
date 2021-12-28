#pragma once

enum class eObjectType : uint32_t {
	SMALL_GATE = 1,
	BIG_GATE = 2,
	MOVING_ESCALATOR = 3,
	STATIC_ESCALATOR = 4,
	SWITCH = 5,
	ESSENCE = 6
};

struct PveObjectData
{
	uint32_t uid;

	eObjectType type;

	Position spawnPosition;
	Position endPosition;

	bool isMoveable;

	uint32_t moveDuration;
	uint32_t moveWait;

	uint32_t linkedObjectId;
};
