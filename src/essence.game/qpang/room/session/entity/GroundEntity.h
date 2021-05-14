#pragma once

#include <cstdint>

struct GroundEntity
{
	uint32_t entityId;
	time_t destroyalTime;
	float x, y, z;
};