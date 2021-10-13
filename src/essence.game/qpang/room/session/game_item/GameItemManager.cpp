#include "GameItemManager.h"

#include "ConfigManager.h"

#include "qpang/Game.h"
#include "qpang/room/session/RoomSession.h"
#include "qpang/room/tnl/net_events/server/gc_game_item.hpp"

void GameItemManager::initialize(std::shared_ptr<RoomSession> roomSession)
{
	m_mapBounds = mapBounds[roomSession->getRoom()->getMap()];
	m_roomSession = roomSession;
	m_areSkillsEnabled = roomSession->getRoom()->isSkillsEnabled();
	m_isEventEligible = (CONFIG_MANAGER->getInt("EVENT_ACTIVE") == 1) 
		&& mapBounds.find(roomSession->getRoom()->getMap()) != mapBounds.cend() 
		&& roomSession->getRoom()->getPassword().empty();

	const auto spawns = Game::instance()->getSpawnManager()->getItemSpawns(roomSession->getRoom()->getMap());

	for (size_t i = 0; i < spawns.size(); i++)
	{
		m_items[i] = GameItemSpawn{
			i,
			getRandomItem(),
			NULL,
			spawns[i],
		};
	}

	m_isReady = true;
	m_initTime = time(NULL);
}

void GameItemManager::tick()
{
	if (!m_isReady)
		return;

	const auto currTime = time(NULL);

	for (auto& [id, item] : m_items)
	{
		if (item.lastPickUpTime == NULL)
			continue;
		else if (item.lastPickUpTime + RESPAWN_INTERVAL < currTime)
		{
			item.lastPickUpTime = NULL;
			item.itemId = getRandomItem();

			auto items = std::vector<GCGameItem::Item>({
				{
					item.itemId,
					item.spawnId,
					item.spawn.x,
					item.spawn.y,
					item.spawn.z,
				}
			});
			
			m_roomSession->relayPlaying<GCGameItem>(6, items, 0);
		}
	}

	if(((!m_eventItemsSpawned && m_initTime + 30 < currTime) || (m_eventItemsSpawned && m_timeSinceEventItemSpawn + EVENT_ITEM_BATCH_INTERVAL < currTime)) && m_isEventEligible)
	{
		m_eventItemsSpawned = true;
		m_timeSinceEventItemSpawn = currTime;
		auto items = std::vector<GCGameItem::Item>();
		auto itemCount = m_roomSession->getPlayingPlayers().size() * 2;
		
		if (itemCount > 30) // too many at a time will crash the game
			itemCount = 30;

		for (int i = 0; i < itemCount; i++)
		{
			uint32_t id = std::rand() % 16777215 + 5000;
			items.push_back(GCGameItem::Item
				{
					EVENT_ITEM,
					id,
					F32(((float)std::rand()) / RAND_MAX * m_mapBounds.x - m_mapBounds.x / 2),
					F32(((float)std::rand()) / RAND_MAX * 400.0),
					F32(((float)std::rand()) / RAND_MAX * m_mapBounds.z - m_mapBounds.z / 2),
				});
			m_eventItems[id] = false;
		}

		m_roomSession->relayPlaying<GCGameItem>(6, items, 0);
	}
}

void GameItemManager::syncPlayer(std::shared_ptr<RoomSessionPlayer> player)
{
	std::vector<GCGameItem::Item> items;
	
	for (auto& [id, item] : m_items)
		if (item.itemId != NULL)
			items.push_back(GCGameItem::Item{ item.itemId, item.spawnId, item.spawn.x, item.spawn.y, item.spawn.z });
	
	player->post(new GCGameItem(6, items, 0));
}

void GameItemManager::reset()
{
	m_roomSession.reset();
	m_items.clear();
	m_eventItems.clear();
}

uint32_t GameItemManager::getRandomItem()
{
	const auto index = std::rand() % possibleItems.size();

	auto item = possibleItems[index];

	if (item == ID::GREEN_MEDKIT && !m_roomSession->getGameMode()->isTeamMode())
		item = ID::RED_MEDKIT;
	else if (item == ID::SKILL_CARD && !m_roomSession->getRoom()->isSkillsEnabled())
		item = ID::AMMO_CLIP;
	else if (item == ID::AMMO_CLIP && m_roomSession->getRoom()->isMeleeOnly())
		item = ID::RED_MEDKIT;

	return item;
}

void GameItemManager::onPickUp(std::shared_ptr<RoomSessionPlayer> player, uint32_t spawnId)
{
	const auto it = m_items.find(spawnId);

	if (it == m_items.cend())
		return onPickUpEventItem(player, spawnId);

	auto& item = it->second;

	auto itemId = item.itemId;

	if (itemId == NULL)
		return;

	if (itemId == RED_MEDKIT || itemId == GREEN_MEDKIT || itemId == AMMO_CLIP || itemId == SKILL_CARD)
	{
		const auto isPublicEnemyMode = player->getRoomSession()->getGameMode()->isPublicEnemyMode();
		const auto playerIsPublicEnemy = player->getRoomSession()->getCurrentlySelectedTag() == player->getPlayer()->getId();

		if ((isPublicEnemyMode && playerIsPublicEnemy) && (itemId == RED_MEDKIT || itemId == GREEN_MEDKIT) || itemId == SKILL_CARD)
		{
			// Cancel pickup for public enemy.
			return;
		}

		const auto identifier = mappedItems[item.itemId]->onPickUp(player);

		m_roomSession->relayPlaying<GCGameItem>(1, player->getPlayer()->getId(), itemId, item.spawnId, identifier);

		item.itemId = NULL;
		item.lastPickUpTime = time(NULL);
	}
}

void GameItemManager::onPickUpEventItem(std::shared_ptr<RoomSessionPlayer> player, uint32_t id)
{
	const auto it = m_eventItems.find(id);

	if (it == m_eventItems.cend())
		return;

	if (m_eventItems[id])
		return;

	const auto identifier = mappedItems[EVENT_ITEM]->onPickUp(player);
	m_roomSession->relayPlaying<GCGameItem>(1, player->getPlayer()->getId(), EVENT_ITEM, id, identifier);
	m_eventItems[id] = true;
}

void GameItemManager::spawnItem(const GameItemManager::GameItemSpawn& item)
{
	std::vector<GCGameItem::Item> items = {
		GCGameItem::Item {
			item.itemId,
			item.spawnId,
			item.spawn.x,
			item.spawn.y,
			item.spawn.z,
		}
	};
	
	m_roomSession->relayPlaying<GCGameItem>(6, items, 0);
}
