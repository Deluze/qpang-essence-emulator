#pragma once

#include <cstdint>
#include "Position.h"

enum class eItemId : uint32_t
{
	NONE = 0,
	AMMO_CLIP = 1191182337,
	RED_MEDKIT = 1191182338,
	GOLDEN_COIN = 1191182352,
	SILVER_COIN = 1191182353,
	BRONZE_COIN = 1191182354
};

enum class eItemSpawnType : uint8_t
{
	COIN = 0,
	AMMO_OR_MEDKIT = 1,
};

struct WeightedItem
{
	eItemId itemId;
	uint32_t weight;
};

struct PveItemData
{
	eItemSpawnType itemSpawnType;
	Position position;
};