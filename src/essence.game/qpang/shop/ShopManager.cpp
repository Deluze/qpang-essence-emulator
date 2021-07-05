#include "ShopManager.h"

#include <iostream>

#include "qpang/player/Player.h"
#include "qpang/player/inventory/InventoryManager.h"
#include "qpang/player/inventory/InventoryCard.h"

#include "packets/lobby/outgoing/shop/CardPurchaseComplete.h"
#include "packets/lobby/outgoing/shop/ShopItems.h"

void ShopManager::initialize()
{
	m_items.clear();
	m_orderedItems.clear();
	
	const auto stmt = DATABASE->prepare("SELECT * FROM `items`");
	const auto res = stmt->fetch();

	std::cout << "Loading shop items.\n";

	while (res->hasNext())
	{
		ShopItem item{
			res->getInt("seq_id"),
			res->getInt("item_id"),
			res->getFlag("pay_type"),
			res->getInt("price"),
			res->getInt("sold_count"),
			res->getInt("stock"),
			res->getTiny("status"),
			res->getTiny("type"),
			res->getTiny("use_up"),
			res->getShort("period"),
			res->getShort("level"),
		};

		m_items[item.seqId] = item;
		m_orderedItems.push_back(item);
		
		res->next();
	}

	std::cout << "Loaded " << m_items.size() << " items.\n";
}

std::vector<ShopItem> ShopManager::list()
{
	std::vector<ShopItem> items;

	for (const auto& [id, item] : m_items)
		items.push_back(item);

	return items;
}

ShopItem ShopManager::get(uint32_t seqId)
{
	auto it = m_items.find(seqId);
	
	return it != m_items.cend() ? (*it).second : ShopItem{};
}

bool ShopManager::exists(uint32_t seqId)
{
	return m_items.find(seqId) != m_items.cend();
}

void ShopManager::buy(std::shared_ptr<Player> player, uint32_t seqId)
{
	// TODO; packages
	assert(player != nullptr);

	if (!exists(seqId))
		return;

	if (!player->getInventoryManager()->hasSpace())
		return;

	auto& shopItem = get(seqId);

	if (shopItem.soldCount >= shopItem.stock && shopItem.stock != 9999)
		return;

	const auto money = shopItem.isCash ? player->getCash() : player->getDon();
	const auto hasEnough = money >= shopItem.price;

	if (!hasEnough)
		return;

	auto card = InventoryCard::fromShopItem(shopItem);

	auto stmt = DATABASE->prepare(
		"INSERT INTO player_items (player_id, item_id, period, period_type, type, active, opened, giftable, boosted, boost_level, time)" \
		"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

	stmt->bindInteger(player->getId());
	stmt->bindInteger(card.itemId);
	stmt->bindInteger(card.period);
	stmt->bindInteger(card.periodType);
	stmt->bindInteger(card.type);
	stmt->bindInteger(card.isActive);
	stmt->bindInteger(card.isOpened);
	stmt->bindInteger(card.isGiftable);
	stmt->bindInteger(card.boostLevel > 0);
	stmt->bindInteger(card.boostLevel);
	stmt->bindLong(time(NULL));

	stmt->execute();

	ShopItem* itemPtr = &m_items[seqId];
	itemPtr->soldCount++;

	auto stmt2 = DATABASE->prepare("UPDATE items SET sold_count = ? WHERE seq_id = ?");
	stmt2->bindInteger(itemPtr->soldCount);
	stmt2->bindInteger(itemPtr->seqId);

	stmt2->execute();

	auto cardId = stmt->getLastInsertId();

	card.id = cardId;
	card.playerOwnerId = player->getId();

	shopItem.isCash ? player->removeCash(shopItem.price) : player->removeDon(shopItem.price);

	player->getInventoryManager()->addCard(card);

	player->send(ShopItems(list()));
	player->send(CardPurchaseComplete(shopItem, { card }, shopItem.isCash ? player->getCash() : player->getDon()));
}
