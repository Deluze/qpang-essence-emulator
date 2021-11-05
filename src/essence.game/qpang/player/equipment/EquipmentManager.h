#pragma once

#include <cstdint>
#include <mutex>
#include <unordered_map>
#include <array>
#include <memory>

#include "qpang/player/inventory/InventoryCard.h"

class Player;
class RoomSessionPlayer;

class EquipmentManager
{
public:
	void initialize(std::shared_ptr<Player> player, uint16_t playerId);

	std::vector<uint16_t> getUnlockedCharacters();

	std::array<uint64_t, 13> getEquipmentByCharacter(uint16_t characterId);
	std::array<uint64_t, 9> getArmorByCharacter(uint16_t characterId);
	std::array<uint32_t, 9> getArmorItemIdsByCharacter(uint16_t characterId);
	std::array<uint32_t, 4> getWeaponItemIdsByCharacter(uint16_t characterId);
	std::array<uint64_t, 4> getWeaponsByCharacter(uint16_t characterId);
	std::array<InventoryCard, 3> getEquippedSkillCards();

	void removeFunctionCard(uint64_t cardId);
	void removeSkillCard(uint64_t cardId);
	void unequipItem(uint64_t cardId);
	
	void addFunctionCard(uint64_t cardId);
	void setFunctionCards(const std::vector<uint64_t>& cards);

	void setSkillCardIds(const std::vector<uint64_t>& skillCardIds);

	void setEquipmentForCharacter(uint16_t character, std::array<uint64_t, 13> equip);
	void setWeapons(uint16_t character, const std::array<uint64_t, 4>& weapons);
	void setArmor(uint16_t character, const std::array<uint64_t, 9>& armor);
	
	uint16_t characterIndexToId(const uint16_t characterIndex);
	bool hasEquipped(const uint64_t cardId);
	bool hasEquipped(const uint64_t cardId, const uint16_t character);
	uint32_t getDefaultWeapon();
	bool hasMeleeWeapon();

	uint16_t getBaseHealth();
	uint16_t getBonusHealth();
	bool hasFunctionCard(uint32_t functionId);

	uint8_t getExtraAmmoForWeaponIndex(uint8_t index);
	uint32_t getEquippedBooster();

	void finishRound(const std::shared_ptr<RoomSessionPlayer>& player);
	void save();
	void close();
private:
	std::unordered_map<uint16_t, std::array<uint64_t, 13>> m_equips;
	std::vector<uint16_t> m_unlockedCharacters;
	std::mutex m_mx;

	std::vector<uint64_t> m_skillCardIds;
	std::recursive_mutex m_skillCardMx;


	std::weak_ptr<Player> m_player;

	std::vector<uint64_t> m_functionCards;
	std::recursive_mutex m_functionCardMx;
};