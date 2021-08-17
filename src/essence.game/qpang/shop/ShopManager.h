#pragma once

#include <unordered_map>

#include "InventoryCard.h"
#include "core/Emulator.h"
#include "core/database/Database.h"

#include "qpang/shop/ShopItem.h"

class Player;

class ShopManager
{
public:
	void initialize();
	std::vector<ShopItem> list();
	ShopItem get(uint32_t seqId);
	bool exists(uint32_t seqId);

	InventoryCard buy(const std::shared_ptr<Player>& player, uint32_t seqId, bool updateShopItems);
private:
	std::unordered_map<uint32_t, ShopItem> m_items;
	std::vector<ShopItem> m_orderedItems;
};