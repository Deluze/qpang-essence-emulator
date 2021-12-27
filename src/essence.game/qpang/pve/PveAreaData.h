#pragma once

#include "Position.h"

struct PveAreaData
{
	uint32_t uid;
	Position minBound;
	Position maxBound;
	uint32_t floorNumber;
};
