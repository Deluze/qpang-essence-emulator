#pragma once

#include <cstdint>

struct ShopItem
{
	uint32_t seqId;
	uint32_t itemId;
	bool isCash;
	uint32_t price;
	uint32_t soldCount;
	uint32_t stock;
	uint8_t shopCategory;
	uint8_t type;
	uint8_t periodType;
	uint16_t period;
	uint16_t minLevel;
};