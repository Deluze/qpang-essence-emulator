#pragma once

#include <unordered_map>

#include "PveItemData.h"
#include "PveNpcData.h"
#include "PveObject.h"

class PveManager
{
public:
	/**
	 * \brief Initializes the pve manager (npcs and objects).
	 */
	void initialize();

	/**
	 * \brief Retrieves the npcs for the given map.
	 * \param mapId The map id.
	 * \return A vector filled with npcs.
	 */
	std::vector<PveNpcData> getNpcDataByMapId(uint8_t mapId);

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
	static std::vector<NpcBodyPart> getBodyPartsByNpcPrimaryKey(uint32_t npcPrimaryKey);

	/**
	 * \brief Retrieves all pve objects from the database and stores them.
	 */
	void initializeObjects();

	/**
	 * \brief Retrieves all pve items from the database and stores them.
	 */
	void initializeItems();

	std::unordered_map<uint32_t, std::vector<PveNpcData>> m_npcData{};
	std::unordered_map<uint32_t, std::vector<PveObjectData>> m_objectData{};
	std::unordered_map<uint32_t, std::vector<PveItemData>> m_itemData{};
};
