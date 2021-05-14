#pragma once

#include <cstdint>
#include <ctime>

#include "qpang/shop/ShopItem.h"

struct InventoryCard
{
	uint64_t id = 0;
	uint32_t playerOwnerId = 0;
	uint32_t itemId = 0;
	uint8_t type = 0;
	uint8_t periodType = 0;
	uint16_t period = 0;
	bool isActive = false;
	bool isOpened = false;
	bool isGiftable = false;
	uint8_t boostLevel = 0;
	uint32_t timeCreated = 0;

	static InventoryCard fromShopItem(const ShopItem& item)
	{
		return {
			NULL,
			NULL,
			item.itemId,
			item.type,
			item.periodType,
			item.period,
			false,
			true,
			true,
			0,
			static_cast<uint32_t>(time(NULL)),
		};
	}
};