#pragma once

#include <unordered_map>

#include "InventoryCard.h"
#include "Player.h"
#include "core/database/Database.h"

#include "qpang/shop/ShopItem.h"

class Player;

class ShopManager
{
public:
	void initialize();

	ShopItem get(uint32_t seqId);
	bool exists(uint32_t seqId);

	std::vector<ShopItem> list();
	void sendShopItems(const Player::Ptr& player);

	InventoryCard buy(const std::shared_ptr<Player>& player, uint32_t seqId, bool updateShopItems);
private:
	std::unordered_map<uint32_t, ShopItem> m_items;
	std::vector<ShopItem> m_orderedItems;

	template<typename T>
	std::vector<T> slice(std::vector<T> const& v, uint32_t m, uint32_t n)
	{
		auto first = v.cbegin() + m;
		auto last = v.cbegin() + n + 1;

		std::vector<T> vec(first, last);
		return vec;
	}
};