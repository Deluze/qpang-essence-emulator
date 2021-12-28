#pragma once

#include <cstdint>
#include "Position.h"

enum class eItemType : uint32_t
{
	NONE = 0,
	AMMO_CLIP = 1191182337,
	RED_MEDKIT = 1191182338,
	GOLDEN_COIN = 1191182352,
	SILVER_COIN = 1191182353,
	BRONZE_COIN = 1191182354
};

struct PveItemData
{
	uint32_t itemId;
	Position position;
};