#include "GameItemManager.h"

#include "ConfigManager.h"

#include "qpang/Game.h"
#include "qpang/room/session/RoomSession.h"
#include "qpang/room/tnl/net_events/server/gc_game_item.hpp"

// TODO: Refactor this class, it's methods and variables.
void GameItemManager::initialize(std::shared_ptr<RoomSession> roomSession)
{
	m_roomSession = roomSession;

	m_mapBounds = mapBounds[m_roomSession->getRoom()->getMap()];
	m_areSkillsEnabled = m_roomSession->getRoom()->isSkillsEnabled();

	m_isEventEligible = (CONFIG_MANAGER->getInt("EVENT_ACTIVE") == 1)
		&& mapBounds.find(m_roomSession->getRoom()->getMap()) != mapBounds.cend()
		&& m_roomSession->getRoom()->getPassword().empty();

	const auto itemSpawns = Game::instance()->getSpawnManager()->getItemSpawns(m_roomSession->getRoom()->getMap());

	for (const auto& itemSpawn : itemSpawns)
	{
		const auto randomItemId = getRandomItem();
		const auto randomSpawnId = (uint32_t)(rand() * std::numeric_limits<uint32_t>::max());

		m_items.push_back(
			GameItemSpawn
			{
				randomSpawnId,
				randomItemId,
				NULL,
				itemSpawn
			}
		);
	}

	m_isReady = true;
	m_initTime = time(NULL);
}

void GameItemManager::tick()
{
	if (!m_isReady)
	{
		return;
	}

	const auto currTime = time(NULL);

	for (auto& item : m_items)
	{
		if (item.lastPickUpTime == NULL)
		{
			continue;
		}
		else if (item.lastPickUpTime + RESPAWN_INTERVAL < currTime)
		{
			item.lastPickUpTime = NULL;
			item.itemId = getRandomItem();

			const auto gameItem = GCGameItem::Item{
				item.itemId,
				item.spawnId,
				item.spawn.x,
				item.spawn.y,
				item.spawn.z,
			};

			const auto items = std::vector<GCGameItem::Item>({ gameItem });

			m_roomSession->relayPlaying<GCGameItem>(GCGameItem::CMD::SPAWN_GAME_ITEM, items, 0);
		}
	}


	// TODO: The next bits of code are about event items, considering to put it into its own function.
	if (((!m_eventItemsSpawned && m_initTime + 30 < currTime) || (m_eventItemsSpawned && m_timeSinceEventItemSpawn + EVENT_ITEM_BATCH_INTERVAL < currTime)) && m_isEventEligible)
	{
		m_eventItemsSpawned = true;
		m_timeSinceEventItemSpawn = currTime;

		auto items = std::vector<GCGameItem::Item>();
		auto itemCount = m_roomSession->getPlayingPlayers().size() * 2;

		// Limit the itemCount to 30, too many will crash the game.
		if (itemCount > 30)
		{
			itemCount = 30;
		}

		for (size_t i = 0; i < itemCount; i++)
		{
			uint32_t eventItemUid = rand() % 16777215 + 5000;

			const auto eventItem = GCGameItem::Item
			{
				EVENT_ITEM,
				eventItemUid,
				F32(((float)std::rand()) / RAND_MAX * m_mapBounds.x - m_mapBounds.x / 2),
				F32(((float)std::rand()) / RAND_MAX * 400.0),
				F32(((float)std::rand()) / RAND_MAX * m_mapBounds.z - m_mapBounds.z / 2),
			};

			items.push_back(eventItem);

			m_eventItems[eventItemUid] = false;
		}

		m_roomSession->relayPlaying<GCGameItem>(GCGameItem::CMD::SPAWN_GAME_ITEM, items, 0);
	}
}

void GameItemManager::syncPlayer(std::shared_ptr<RoomSessionPlayer> player)
{
	std::vector<GCGameItem::Item> items;

	for (const auto& item : m_items)
	{
		if (item.itemId != NULL)
		{
			const auto gameItem = GCGameItem::Item
			{
				item.itemId,
				item.spawnId,
				item.spawn.x,
				item.spawn.y,
				item.spawn.z
			};

			items.push_back(gameItem);
		}
	}

	player->post(new GCGameItem(GCGameItem::CMD::SPAWN_GAME_ITEM, items, 0));
}

void GameItemManager::reset()
{
	m_roomSession.reset();
	m_items.clear();
	m_eventItems.clear();
}

uint32_t GameItemManager::getRandomItem()
{
	const auto isSkillsEnabled = m_roomSession->getRoom()->isSkillsEnabled();

	const auto randomIndex = (isSkillsEnabled)
		? rand() % possibleItemsWithSkillsEnabled.size()
		: rand() % possibleItems.size();

	auto itemId = (isSkillsEnabled)
		? possibleItemsWithSkillsEnabled[randomIndex]
		: possibleItems[randomIndex];

	const auto isTeamMode = m_roomSession->getGameMode()->isTeamMode();
	const auto isMeleeOnly = m_roomSession->getRoom()->isMeleeOnly();

	switch (itemId)
	{
	case ID::GREEN_MEDKIT:
		if (!isTeamMode)
		{
			itemId = ID::RED_MEDKIT;
		}

		break;
	case ID::AMMO_CLIP:
		if (isMeleeOnly)
		{
			itemId = ID::RED_MEDKIT;
		}
		break;
	default:
		break;
	}

	return itemId;
}

void GameItemManager::onPickUp(std::shared_ptr<RoomSessionPlayer> player, uint32_t spawnId)
{
	GameItemSpawn* gameItemSpawn = nullptr;

	for (auto& item : m_items)
	{
		if (item.spawnId == spawnId)
		{
			gameItemSpawn = &item;

			break;
		}
	}

	if (gameItemSpawn == nullptr)
	{
		onPickUpEventItem(player, spawnId);

		return;
	}

	const auto itemId = gameItemSpawn->itemId;

	if (itemId == NULL)
	{
		return;
	}

	if (itemId == RED_MEDKIT || itemId == GREEN_MEDKIT || itemId == AMMO_CLIP || itemId == SKILL_CARD)
	{
		const auto isPublicEnemyMode = player->getRoomSession()->getGameMode()->isPublicEnemyMode();
		const auto playerIsPublicEnemy = player->getRoomSession()->getCurrentlySelectedTag() == player->getPlayer()->getId();

		if ((isPublicEnemyMode && playerIsPublicEnemy) && (itemId == RED_MEDKIT || itemId == GREEN_MEDKIT || itemId == SKILL_CARD))
		{
			// Cancel pickup for public enemy.
			return;
		}

		const auto identifier = mappedItems[itemId]->onPickUp(player);

		m_roomSession->relayPlaying<GCGameItem>(GCGameItem::CMD::PICKUP_GAME_ITEM, player->getPlayer()->getId(), itemId, gameItemSpawn->spawnId, identifier);
		gameItemSpawn->itemId = NULL;
		gameItemSpawn->spawnId = (uint32_t)(rand() * std::numeric_limits<uint32_t>::max());
		gameItemSpawn->lastPickUpTime = time(NULL);
	}

	if (player->getSkillManager()->hasActiveSkill())
	{
		const auto shouldDisableOnGameItemPickup = player->getSkillManager()->getActiveSkill()->shouldDisableOnGameItemPickup();

		if (shouldDisableOnGameItemPickup)
		{
			player->getSkillManager()->deactivateSkill();
		}
	}
}

void GameItemManager::onPickUpEventItem(std::shared_ptr<RoomSessionPlayer> player, uint32_t id)
{
	const auto it = m_eventItems.find(id);

	if (it == m_eventItems.cend())
	{
		return;
	}

	if (m_eventItems[id])
	{
		return;
	}

	const auto identifier = mappedItems[EVENT_ITEM]->onPickUp(player);

	m_roomSession->relayPlaying<GCGameItem>(GCGameItem::CMD::PICKUP_GAME_ITEM, player->getPlayer()->getId(), EVENT_ITEM, id, identifier);

	m_eventItems[id] = true;

	if (player->getSkillManager()->hasActiveSkill())
	{
		const auto shouldDisableOnGameItemPickup = player->getSkillManager()->getActiveSkill()->shouldDisableOnGameItemPickup();

		if (shouldDisableOnGameItemPickup)
		{
			player->getSkillManager()->deactivateSkill();
		}
	}
}