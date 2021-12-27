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
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	m_items.clear();
	m_spawnedItems.clear();

	const auto itemData = Game::instance()->getPveManager()->getItemDataByMapId(roomSession->getRoom()->getMap());

	for (const auto& [itemId, position] : itemData)
	{
		const auto item = PveItem(itemId, position);

		m_items.push_back(item);
	}
}

void RoomSessionPveItemManager::spawnInitializedItems()
{
	for (const auto& item : m_items)
	{
		spawnItem(std::make_shared<PveItem>(item));
	}
}

uint32_t RoomSessionPveItemManager::spawnItem(const std::shared_ptr<PveItem>& item)
{
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

	switch (const auto itemType = static_cast<eItemType>(item->getItemId()))
	{
	case eItemType::AMMO_CLIP:
		handleAmmoPickup(roomSessionPlayer);
		break;
	case eItemType::RED_MEDKIT:
		handleMedkitPickup(roomSessionPlayer);
		break;
	case eItemType::BRONZE_COIN:
	case eItemType::SILVER_COIN:
	case eItemType::GOLDEN_COIN:
		handleCoinPickup(roomSessionPlayer, itemType);
		break;
	case eItemType::NONE:
		return;
	}

	roomSession->relayPlaying<GCGameItem>(1, playerId, item->getItemId(), uid, 0);

	m_spawnedItems.erase(uid);
}

void RoomSessionPveItemManager::removeAll()
{
	m_items.clear();
	m_spawnedItems.clear();
}

std::shared_ptr<PveItem> RoomSessionPveItemManager::findItemByUid(const uint32_t itemUid)
{
	const auto it = m_spawnedItems.find(itemUid);

	if (it == m_spawnedItems.end())
	{
		return nullptr;
	}

	return it->second;
}

void RoomSessionPveItemManager::onPlayerSync(const std::shared_ptr<RoomSessionPlayer>& session) const
{
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

void RoomSessionPveItemManager::handleCoinPickup(const RoomSessionPlayer::Ptr& roomSessionPlayer, const eItemType itemType)
{
	auto coins = 0;

	switch (itemType)
	{
	case eItemType::BRONZE_COIN:
		coins = 1;
		break;
	case eItemType::SILVER_COIN:
		coins = 10;
		break;
	case eItemType::GOLDEN_COIN:
		coins = 100;
		break;
	case eItemType::NONE:
	case eItemType::AMMO_CLIP:
	case eItemType::RED_MEDKIT:
		return;
	}

	roomSessionPlayer->getPlayer()->addCoins(coins);
}