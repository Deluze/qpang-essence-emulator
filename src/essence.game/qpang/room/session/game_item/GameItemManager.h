#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <memory>

#include "qpang/spawn/Spawn.h"

#include "qpang/room/session/game_item/items/RedMedKit.h"
#include "qpang/room/session/game_item/items/AmmoClip.h"
#include "qpang/room/session/game_item/items/GreenMedKit.h"
#include "qpang/room/session/game_item/items/SkillBook.h"
#include "qpang/room/session/game_item/items/EventItem.h"

constexpr auto RESPAWN_INTERVAL = 15;
constexpr auto EVENT_ITEM_BATCH_INTERVAL = 200;

class GameItem;
class RoomSession;
class RoomSessionPlayer;

enum ID : uint32_t
{
	AMMO_CLIP = 1191182337,
	RED_MEDKIT = 1191182338,
	EVENT_ITEM = 1191182344,
	GREEN_MEDKIT = 1191182350,
	SKILL_CARD = 1191182351,
	GOLD_COIN = 1191182352,
	SILVER_COIN = 1191182353,
	BRONZE_COIN = 1191182354
};

static std::vector<uint32_t> possibleItemsWithSkillsEnabled =
{
	SKILL_CARD,
	AMMO_CLIP,
	GREEN_MEDKIT,
	RED_MEDKIT,
	SKILL_CARD,
};

static std::vector<uint32_t> possibleItems =
{
	AMMO_CLIP,
	RED_MEDKIT,
	AMMO_CLIP,
	GREEN_MEDKIT,
};

static std::unordered_map<uint32_t, GameItem*> mappedItems =
{
	{RED_MEDKIT, new RedMedKit() },
	{AMMO_CLIP, new AmmoClip() },
	{GREEN_MEDKIT, new GreenMedKit() },
	{SKILL_CARD, new SkillBook() },
	{EVENT_ITEM, new EventItem() },
};

static std::unordered_map<uint32_t, Spawn> mapBounds =
{
	{ 0, Spawn({48, 0, 48}) }, // Garden
	{ 1, Spawn({48, 0, 48}) }, // Diorama
	{ 2, Spawn({64, 0, 64}) }, // Skycastle
	{ 3, Spawn({34, 0, 34}) }, // Ossyria
	{ 4, Spawn({62, 0, 18}) }, // Dollhouse
	{ 6, Spawn({58, 0, 38}) }, // City
	{ 8, Spawn({58, 0, 60}) }, // Bunker
	{ 9, Spawn({58, 0, 58}) }, // Temple
	{ 11, Spawn({37, 0, 92}) }, // Bridge temporarily disabled
	{ 12, Spawn({66, 0, 66}) }, // Castaway
};

class GameItemManager
{
	struct GameItemSpawn
	{
		uint32_t spawnId;
		uint32_t itemId;
		time_t lastPickUpTime;
		Spawn spawn;
	};

public:
	void initialize(std::shared_ptr<RoomSession> roomSession);

	void tick();
	void syncPlayer(std::shared_ptr<RoomSessionPlayer> player);
	void reset();
	uint32_t getRandomItem();

	void onPickUp(std::shared_ptr<RoomSessionPlayer> player, uint32_t spawnId);
	void onPickUpEventItem(std::shared_ptr<RoomSessionPlayer> player, uint32_t id);
private:
	std::vector<GameItemSpawn> m_items;
	std::unordered_map<uint32_t, bool> m_eventItems;
	bool m_areSkillsEnabled = false;
	bool m_isReady = false;
	bool m_eventItemsSpawned = false;
	bool m_isEventEligible = false;
	uint32_t m_initTime = NULL;
	uint32_t m_timeSinceEventItemSpawn = NULL;
	Spawn m_mapBounds;

	std::shared_ptr<RoomSession> m_roomSession;
};