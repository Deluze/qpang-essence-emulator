#pragma once

#include <unordered_map>

#include "PveAreaData.h"
#include "PveItemData.h"
#include "PveNpcData.h"
#include "PveObject.h"

class PveManager
{
public:
	/**
	 * \brief Initializes the pve manager (npc's and objects).
	 */
	void initialize();

	/**
	 * \brief Retrieves the npc's for the given map.
	 * \param mapId The map id.
	 * \return A vector filled with npc's.
	 */
	std::vector<PveNpcData> getNpcDataByMapId(uint8_t mapId);

	/**
	 * \brief Retrieves the npc's waves for the given map.
	 * \param mapId The map id.
	 * \return A data set containing wave number and pve npc data.
	 */
	std::unordered_map<uint8_t, std::vector<PveNpcWaveData>> getNpcWaveData(uint8_t mapId);

	/**
	 * \brief Retrieves the objects for the given map.
	 * \param mapId The map id.
	 * \return A vector filled with objects.
	 */
	std::vector<PveObjectData> getObjectDataByMapId(uint8_t mapId);

	/**
	 * \brief Retrieves the items for the given map.
	 * \param mapId The map id.
	 * \return A vector filled with items.
	 */
	std::vector<PveItemData> getItemDataByMapId(uint8_t mapId);

	/**
	 * \brief Retrieves the areas for the given map.
	 * \param mapId The map id.
	 * \return A vector filled with areas.
	 */
	std::vector<PveAreaData> getAreaDataByMapId(uint8_t mapId);
private:
	/**
	 * \brief Retrieves all pve_npc_spawns from the database and stores them.
	 */
	void initializeNpcs();

	/**
	 * \brief Retrieves all of the loot drops for the given npc.
	 * \param npcPrimaryKey The primary key (id) of the npc in the pve_npcs database table.
	 * \return A vector filled with npc loot drops.
	 */
	static std::vector<NpcLootDrop> getLootDropsByNpcPrimaryKey(uint32_t npcPrimaryKey);

	/**
	 * \brief Retrieves all of the body parts for the given npc.
	 * \param npcPrimaryKey The primary key (id) of the npc in the pve_npcs database table.
	 * \return A vector filled with npc body parts.
	 */
	static std::vector<std::shared_ptr<NpcBodyPart>> getBodyPartsByNpcPrimaryKey(uint32_t npcPrimaryKey);

	/**
	 * \brief Retrieves all pve objects from the database and stores them.
	 */
	void initializeObjects();

	/**
	 * \brief Retrieves all pve items from the database and stores them.
	 */
	void initializeItems();

	/**
	 * \brief Retrieves all areas from the database and stores them.
	 */
	void initializeAreas();

	/**
	 * \brief Retrieves all npc waves from the database and stores them.
	 */
	void initializeNpcWaves();

	std::unordered_map<uint32_t, std::vector<PveNpcData>> m_npcData{};
	std::unordered_map<uint32_t, std::vector<PveObjectData>> m_objectData{};
	std::unordered_map<uint32_t, std::vector<PveItemData>> m_itemData{};
	std::unordered_map<uint32_t, std::vector<PveAreaData>> m_areaData{};

	std::unordered_map<uint32_t, std::unordered_map<uint8_t, std::vector<PveNpcWaveData>>> m_npcWaveData{};
};
