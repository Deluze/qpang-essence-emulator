#include "RoomSessionPveItemManager.h"

#include "RoomSessionPlayer.h"

#include "gc_game_item.hpp"
#include "gc_pve_item_init.hpp"

void RoomSessionPveItemManager::initialize(const std::shared_ptr<RoomSession>& roomSession)
{
	m_roomSession = roomSession;
}

void RoomSessionPveItemManager::tick() const
{

}

void RoomSessionPveItemManager::initializeItems()
{
	std::lock_guard<std::recursive_mutex> lg(m_mx);

	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	m_items.clear();
	m_spawnedItems.clear();

	const auto mapId = roomSession->getPveRoundManager()->getMap();
	const auto itemData = Game::instance()->getPveManager()->getItemDataByMapId(mapId);

	for (const auto& [itemSpawnType, position] : itemData)
	{
		const auto item = PveItem(itemSpawnType, position);

		m_items.push_back(item);
	}
}

void RoomSessionPveItemManager::spawnInitializedItems()
{
	std::lock_guard<std::recursive_mutex> lg(m_mx);

	for (const auto& item : m_items)
	{
		spawnWeightedRandomItem(std::make_shared<PveItem>(item));
	}
}

uint32_t RoomSessionPveItemManager::spawnWeightedRandomItem(const std::shared_ptr<PveItem>& item)
{
	std::lock_guard<std::recursive_mutex> lg(m_mx);

	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return 0;
	}

	item->setUid(m_spawnedItems.size() + 1);
	item->setWeightedRandomItemId();
	item->spawn(roomSession);

	m_spawnedItems[item->getUid()] = item;

	return item->getUid();
}

uint32_t RoomSessionPveItemManager::spawnItem(const std::shared_ptr<PveItem>& item)
{
	std::lock_guard<std::recursive_mutex> lg(m_mx);

	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return 0;
	}

	item->setUid(m_spawnedItems.size() + 1);
	item->spawn(roomSession);

	m_spawnedItems[item->getUid()] = item;

	return item->getUid();
}

void RoomSessionPveItemManager::onItemPickup(const uint32_t playerId, const uint32_t uid)
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	const auto roomSessionPlayer = roomSession->find(playerId);

	if (roomSessionPlayer == nullptr)
	{
		return;
	}

	const auto& item = findItemByUid(uid);

	if (item == nullptr)
	{
		return;
	}

	if (item->isPickedUp())
	{
		return;
	}

	switch (const auto itemType = static_cast<eItemId>(item->getItemId()))
	{
	case eItemId::AMMO_CLIP:
		handleAmmoPickup(roomSessionPlayer);
		break;
	case eItemId::RED_MEDKIT:
		handleMedkitPickup(roomSessionPlayer);
		break;
	case eItemId::BRONZE_COIN:
	case eItemId::SILVER_COIN:
	case eItemId::GOLDEN_COIN:
		handleCoinPickup(roomSessionPlayer, itemType);
		break;
	case eItemId::NONE:
		return;
	}

	roomSession->relayPlaying<GCGameItem>(GCGameItem::CMD::PICKUP_GAME_ITEM, playerId, item->getItemId(), uid, 0);

	item->setIsPickedUp(true);
}

void RoomSessionPveItemManager::removeAll()
{
	std::lock_guard<std::recursive_mutex> lg(m_mx);

	m_items.clear();
	m_spawnedItems.clear();
}

std::shared_ptr<PveItem> RoomSessionPveItemManager::findItemByUid(const uint32_t itemUid)
{
	std::lock_guard<std::recursive_mutex> lg(m_mx);

	const auto it = m_spawnedItems.find(itemUid);

	if (it == m_spawnedItems.end())
	{
		return nullptr;
	}

	return it->second;
}

void RoomSessionPveItemManager::onPlayerSync(const std::shared_ptr<RoomSessionPlayer>& session)
{
	std::lock_guard<std::recursive_mutex> lg(m_mx);

	for (const auto& [uid, item] : m_spawnedItems)
	{
		item->spawn(session);
	}
}

void RoomSessionPveItemManager::onStart()
{
	initializeItems();
	spawnInitializedItems();
}

void RoomSessionPveItemManager::handleAmmoPickup(const RoomSessionPlayer::Ptr& roomSessionPlayer)
{
	roomSessionPlayer->getWeaponManager()->refillCurrentWeapon();
}

void RoomSessionPveItemManager::handleMedkitPickup(const RoomSessionPlayer::Ptr& roomSessionPlayer)
{
	roomSessionPlayer->addHealth(50, true);
}

void RoomSessionPveItemManager::handleCoinPickup(const RoomSessionPlayer::Ptr& roomSessionPlayer, const eItemId itemType)
{
	auto coinCount = 0;

	switch (itemType)
	{
	case eItemId::BRONZE_COIN:
		coinCount = 1;
		roomSessionPlayer->increaseBronzeCoinCount();
		break;
	case eItemId::SILVER_COIN:
		coinCount = 10;
		roomSessionPlayer->increaseSilverCoinCount();
		break;
	case eItemId::GOLDEN_COIN:
		coinCount = 100;

		roomSessionPlayer->increaseGoldenCoinCount();
		break;
	case eItemId::NONE:
	case eItemId::AMMO_CLIP:
	case eItemId::RED_MEDKIT:
		return;
	}

	roomSessionPlayer->getPlayer()->addCoins(coinCount);
}