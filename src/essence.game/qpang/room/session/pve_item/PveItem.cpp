#include "PveItem.h"

#include "gc_pve_item_init.hpp"
#include "RandomHelper.h"

PveItem::PveItem(const PveItemData data) :
	m_spawnType(data.itemSpawnType),
	m_position(data.position),
	m_respawnTime(data.respawnTime),
	m_shouldRespawn(data.shouldRespawn)
{
}

PveItem::PveItem(const eItemSpawnType spawnType, const Position& position) :
	m_spawnType(spawnType),
	m_position(position)
{
}

PveItem::PveItem(const uint32_t itemId, const Position& position) :
	m_itemId(itemId),
	m_spawnType(),
	m_position(position)
{
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void PveItem::tick(const std::shared_ptr<RoomSession>& roomSession)
{
	if (m_shouldRespawn)
	{
		const auto currentTime = time(nullptr);

		if (m_lastPickupTime == NULL)
		{
			return;
		}

		if ((m_lastPickupTime + m_respawnTime) < currentTime)
		{
			m_lastPickupTime = NULL;

			roomSession->getPveItemManager()->respawnItem(m_uid);
		}
	}
}

void PveItem::spawn(const std::shared_ptr<RoomSession>& roomSession) const
{
	roomSession->relayPlaying<GCPvEItemInit>(m_itemId, m_uid, m_position);
}

void PveItem::spawn(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer) const
{
	roomSessionPlayer->send<GCPvEItemInit>(m_itemId, m_uid, m_position);
}

void PveItem::setUid(const uint32_t uid)
{
	m_uid = uid;
}

void PveItem::setWeightedRandomItemId()
{
	std::vector<WeightedItem> weightedItems{};

	switch (m_spawnType)
	{
	case eItemSpawnType::COIN:
		weightedItems = m_weightedCoins;
		break;
	case eItemSpawnType::AMMO_OR_MEDKIT:
		weightedItems = m_weightedAmmoAndMedkits;
		break;
	}

	std::vector<eItemId> allItemIds{};

	for (const auto& [itemId, weight] : weightedItems)
	{
		for (uint32_t i = 0; i < weight; i++)
		{
			allItemIds.push_back(itemId);
		}
	}

	const auto randomIndex = RandomHelper::getRandomNumber(0, allItemIds.size() - 1);
	const auto randomItemId = static_cast<uint32_t>(allItemIds[randomIndex]);

	m_itemId = randomItemId;
}

void PveItem::setIsPickedUp(const bool value)
{
	m_isPickedUp = value;
}

void PveItem::setLastPickupTime(const uint32_t time)
{
	m_lastPickupTime = time;
}

uint32_t PveItem::getUid() const
{
	return m_uid;
}

uint32_t PveItem::getItemId() const
{
	return m_itemId;
}

Position PveItem::getPosition() const
{
	return m_position;
}

eItemSpawnType PveItem::getSpawnType() const
{
	return m_spawnType;
}

bool PveItem::isPickedUp() const
{
	return m_isPickedUp;
}
