#include "EquipmentManager.h"

#include "core/Emulator.h"
#include "core/database/Database.h"

#include "qpang/Game.h"

#include "qpang/player/inventory/InventoryManager.h"
#include "qpang/player/Player.h"
#include "qpang/room/session/player/RoomSessionPlayer.h"

#include "packets/lobby/outgoing/equipment/SetWeapons.h"
#include "packets/lobby/outgoing/equipment/SetArmor.h"

#include "packets/lobby/outgoing/inventory/Inventory.h"

void EquipmentManager::initialize(std::shared_ptr<Player> player, uint16_t playerId)
{
	m_player = player;

	m_unlockedCharacters = {
		333,
		343,
		578,
		579,
		850,
		851
	};

	Statement::Ptr stmt = DATABASE->prepare("SELECT * FROM player_equipment WHERE player_id = ?");
	stmt->bindInteger(playerId);
	StatementResult::Ptr res = stmt->fetch();

	std::lock_guard l(m_mx);

	while (res->hasNext())
	{
		std::array<uint64_t, 13> equips{};

		uint16_t characterId = res->getShort("character_id");
		equips[0] = res->getInt("head");
		equips[1] = res->getInt("face");
		equips[2] = res->getInt("body");
		equips[3] = res->getInt("hands");
		equips[4] = res->getInt("legs");
		equips[5] = res->getInt("shoes");
		equips[6] = res->getInt("back");
		equips[7] = res->getInt("side");
		equips[8] = 0; // ?
		equips[9] = res->getInt("primary");
		equips[10] = res->getInt("secondary");
		equips[11] = res->getInt("throw");
		equips[12] = res->getInt("melee");

		m_equips[characterId] = equips;

		res->next();
	}
}

std::vector<uint16_t> EquipmentManager::getUnlockedCharacters()
{
	return {
		m_unlockedCharacters
	};
}

std::array<uint64_t, 13> EquipmentManager::getEquipmentByCharacter(uint16_t characterId)
{
	std::lock_guard l(m_mx);

	const auto it = m_equips.find(characterId);

	if (it == m_equips.cend())
		return {};

	return it->second;
}

std::array<uint64_t, 9> EquipmentManager::getArmorByCharacter(uint16_t characterId)
{
	std::lock_guard l(m_mx);

	auto it = m_equips.find(characterId);

	if (it == m_equips.cend())
		return {};

	std::array<uint64_t, 13> equip = it->second;

	return {
		equip[0],
		equip[1],
		equip[2],
		equip[3],
		equip[4],
		equip[5],
		equip[6],
		equip[7],
		equip[8]
	};
}

std::array<uint32_t, 9> EquipmentManager::getArmorItemIdsByCharacter(uint16_t characterId)
{
	if (const auto player = m_player.lock(); player != nullptr)
	{
		const auto& armor = getArmorByCharacter(characterId);
		std::array<uint32_t, 9> armorItemIds = {};

		for (size_t i = 0; i < armor.size(); i++)
			armorItemIds[i] = player->getInventoryManager()->get(armor[i]).itemId;

		return armorItemIds;
	}

	return {};
}

std::array<uint32_t, 4> EquipmentManager::getWeaponItemIdsByCharacter(uint16_t characterId)
{
	if (const auto player = m_player.lock(); player != nullptr)
	{
		const auto& weapons = getWeaponsByCharacter(characterId);
		std::array<uint32_t, 4> weaponItemIds = {};

		for (size_t i = 0; i < weapons.size(); i++)
			weaponItemIds[i] = player->getInventoryManager()->get(weapons[i]).itemId;

		return weaponItemIds;
	}

	return {};
}

std::array<uint64_t, 4> EquipmentManager::getWeaponsByCharacter(uint16_t characterId)
{
	std::lock_guard l(m_mx);

	auto it = m_equips.find(characterId);

	if (it == m_equips.cend())
		return {};

	const std::array<uint64_t, 13>& equip = it->second;

	return {
		equip[9],
		equip[10],
		equip[11],
		equip[12]
	};
}

std::array<InventoryCard, 3> EquipmentManager::getEquippedSkillCards()
{
	std::lock_guard l(m_mx);

	std::array<InventoryCard, 3> skillCards;

	if (const auto player = m_player.lock(); player != nullptr)
	{
		InventoryManager* inv = player->getInventoryManager();

		for (int i = 0; i < m_skillCardIds.size(); i++)
		{
			skillCards[i] = inv->get(m_skillCardIds[i]);
		}
	}

	return skillCards;
}

std::vector<uint64_t> EquipmentManager::getEquippedSkillCardIds() const
{
	return m_skillCardIds;
}

void EquipmentManager::removeFunctionCard(uint64_t cardId)
{
	std::lock_guard g(m_functionCardMx);

	m_functionCards.erase(std::remove_if(m_functionCards.begin(), m_functionCards.end(),
		[cardId](const uint64_t& lhs)
		{
			return lhs == cardId;
		}
	), m_functionCards.end());
}

void EquipmentManager::removeSkillCard(uint64_t cardId)
{
	std::lock_guard g(m_skillCardMx);

	m_skillCardIds.erase(std::remove_if(m_skillCardIds.begin(), m_skillCardIds.end(),
		[cardId](const uint64_t& lhs)
		{
			return lhs == cardId;
		}
	), m_skillCardIds.end());
}

void EquipmentManager::unequipItem(uint64_t cardId)
{
	std::lock_guard l(m_mx);

	for (size_t i = 0; i < m_unlockedCharacters.size(); i++)
	{
		auto character = m_unlockedCharacters[i];
		auto it = m_equips.find(character);

		if (it == m_equips.cend())
			continue;

		auto& equips = it->second;

		for (size_t y = 0; y < 13; y++)
			if (equips[y] == cardId)
				equips[y] = 0;
	}
}

void EquipmentManager::addFunctionCard(uint64_t cardId)
{
	std::lock_guard g(m_functionCardMx);

	m_functionCards.push_back(cardId);
}

void EquipmentManager::setFunctionCards(const std::vector<uint64_t>& cards)
{
	std::lock_guard g(m_functionCardMx);

	m_functionCards = cards;
}

void EquipmentManager::setSkillCardIds(const std::vector<uint64_t>& skillCardIds)
{
	std::lock_guard g(m_skillCardMx);

	m_skillCardIds = skillCardIds;
}

void EquipmentManager::setEquipmentForCharacter(uint16_t character, std::array<uint64_t, 13> equip)
{
	m_equips[character] = equip;
}

void EquipmentManager::setWeapons(uint16_t character, const std::array<uint64_t, 4>& weapons)
{
	const auto characterId = characterIndexToId(character);

	if (const auto player = m_player.lock(); player != nullptr)
	{
		InventoryManager* inv = player->getInventoryManager();

		for (const auto& weapon : weapons)
		{
			if (weapon == NULL)
			{
				continue;
			}

			if (!Game::instance()->getWeaponManager()->canEquip(inv->get(weapon).itemId, characterId))
			{
				return;
			}

			if (!inv->hasCard(weapon) || inv->isExpired(weapon))
			{
				return;
			}
		}

		m_equips[characterId][9] = weapons[0];
		m_equips[characterId][10] = weapons[1];
		m_equips[characterId][11] = weapons[2];
		m_equips[characterId][12] = weapons[3];

		player->send(SetWeapons(character, weapons));
	}
}

void EquipmentManager::setArmor(uint16_t character, const std::array<uint64_t, 9>& armor)
{
	const auto characterId = characterIndexToId(character);

	if (const auto player = m_player.lock(); player != nullptr)
	{
		InventoryManager* inv = player->getInventoryManager();

		for (const auto& piece : armor)
		{
			if (piece == NULL)
				continue;

			if (!inv->hasCard(piece) || inv->isExpired(piece))
				return;
		}

		m_equips[characterId][0] = armor[0];
		m_equips[characterId][1] = armor[1];
		m_equips[characterId][2] = armor[2];
		m_equips[characterId][3] = armor[3];
		m_equips[characterId][4] = armor[4];
		m_equips[characterId][5] = armor[5];
		m_equips[characterId][6] = armor[6];
		m_equips[characterId][7] = armor[7];

		player->send(SetArmor(character, armor));
	}
}

uint16_t EquipmentManager::characterIndexToId(const uint16_t characterIndex)
{
	switch (characterIndex)
	{
	default:
	case 0:
		return 333;
	case 1:
		return 343;
	case 2:
		return 578;
	case 3:
		return 579;
	case 4:
		return 850;
	case 5:
		return 851;
	}
}

bool EquipmentManager::hasEquipped(const uint64_t cardId)
{
	for (const auto& character : m_unlockedCharacters)
		if (hasEquipped(cardId, character))
			return true;

	return false;
}

bool EquipmentManager::hasEquipped(const uint64_t cardId, const uint16_t character)
{
	const auto& equips = m_equips[character];

	for (const auto& equipment : equips)
		if (equipment == cardId)
			return true;

	for (const auto& skillCard : m_skillCardIds)
		if (skillCard == cardId)
			return true;

	for (const auto& functionCard : m_functionCards)
		if (functionCard == cardId)
			return true;

	return false;
}

uint32_t EquipmentManager::getDefaultWeapon()
{
	if (const auto player = m_player.lock(); player != nullptr)
	{
		const auto characterId = player->getCharacter();

		const auto& weapons = getWeaponItemIdsByCharacter(characterId);

		for (const auto weapon : weapons)
			if (weapon != NULL)
				return weapon;
	}

	return 0;
}

bool EquipmentManager::hasMeleeWeapon()
{
	if (const auto player = m_player.lock(); player != nullptr)
	{
		const auto characterId = player->getCharacter();
		const auto& weapons = getWeaponItemIdsByCharacter(characterId);

		return weapons[3] != NULL;
	}

	return false;
}

uint16_t EquipmentManager::getBaseHealth()
{
	if (auto player = m_player.lock(); player != nullptr)
	{
		switch (player->getCharacter())
		{
		case 850: // sai
			return 130;
		case 851: // uru
			return 140;
		case 578: // kuma
			return 200;
		case 579: // miu miu
			return 80;
		case 343: // hana
			return 100;
		case 333: // ken
			return 110;
		default:
			return 100;
		}
	}

	return 0;
}

uint16_t EquipmentManager::getBonusHealth()
{
	if (auto player = m_player.lock(); player != nullptr)
	{
		auto equip = m_equips[player->getCharacter()];

		switch (player->getInventoryManager()->get(equip[6]).itemId)
		{
		case 1429407489: // green turtle
			return 10;
		case 1429410048: // brown turtle
		case 1429408256: // sidekick
		case 1429414144: // wallie
		case 1429414400: // german
		case 1429430784: // TGS
		case 1429431040: // orange
		case 1429409024: // alpha
		case 1429412097: // yellow helper
		// Other backpacks
		case 1429412101:
		case 1429412102:
		case 1429412103:
		case 1429412104:
		case 1429412105:
		case 1429412106:
		case 1429412107:
		case 1429412108:
		case 1429412109:
		case 1429412098:
		case 1429412099:
		case 1429412100:
			// Squirtle shield
		case 1429410049:
			return 20;
		case 1429415424: // novice back
			return 30;
		default:
			return 0;
		}
	}

	return 0;
}

bool EquipmentManager::hasFunctionCard(uint32_t functionId)
{
	if (auto player = m_player.lock(); player != nullptr)
		for (const auto& functionCard : m_functionCards)
			if (player->getInventoryManager()->get(functionCard).itemId == functionId)
				return true;

	return false;
}

uint8_t EquipmentManager::getExtraAmmoForWeaponIndex(uint8_t index)
{
	if (index == 3)
		return 0; // melee lol

	if (const auto player = m_player.lock(); player != nullptr)
	{
		auto equip = m_equips[player->getCharacter()];

		auto itemId = player->getInventoryManager()->get(equip[6]).itemId;

		if (index == 0 && itemId == 1429409536) // schiet ammo
			return 1;
		if (index == 1 && itemId == 1429409792) // lanceer ammo
			return 1;
		if (index == 2 && itemId == 1429408001) // werp ammo
			return 1;
	}

	return 0;
}

uint32_t EquipmentManager::getEquippedBooster()
{
	if (const auto player = m_player.lock(); player != nullptr)
	{
		const auto& equip = m_equips[player->getCharacter()];
		const auto itemId = player->getInventoryManager()->get(equip[7]).itemId;

		return itemId;
	}

	return 0;
}

void EquipmentManager::finishRound(const std::shared_ptr<RoomSessionPlayer>& session)
{
	auto player = m_player.lock();

	if (player == nullptr)
		return;

	const auto character = session->getCharacter();
	const auto playtime = session->getPlaytime();

	const auto it = m_equips.find(character);

	if (it == m_equips.cend())
		return;

	for (const auto& equipment : it->second)
	{
		player->getInventoryManager()->useCard(equipment, playtime);
	}

	m_functionCardMx.lock();

	for (const auto& function : m_functionCards)
	{
		player->getInventoryManager()->useCard(function, playtime);
	}

	m_functionCardMx.unlock();

	const auto cards = player->getInventoryManager()->list();

	player->getInventoryManager()->sendCards();

	save();
}

void EquipmentManager::save()
{
	if (const auto player = m_player.lock(); player != nullptr)
	{
		for (const auto& character : m_unlockedCharacters)
		{
			const auto it = m_equips.find(character);

			if (it == m_equips.cend())
				continue;

			const auto& equipment = it->second;

			DATABASE_DISPATCHER->dispatch(
				"UPDATE player_equipment " \
				"SET `melee` = ?, `primary` = ?, `secondary` = ?, `throw` = ?, `head` = ?, `face` = ?, `body` = ?, `hands` = ?, `legs` = ?, `shoes` = ?, `back` = ?, `side` = ? " \
				"WHERE `character_id` = ? AND `player_id` = ?",
				{
					{equipment[12], true},
					{equipment[9], true},
					{equipment[10], true},
					{equipment[11], true},
					{equipment[0], true},
					{equipment[1], true},
					{equipment[2], true},
					{equipment[3], true},
					{equipment[4], true},
					{equipment[5], true},
					{equipment[6], true},
					{equipment[7], true},
					character,
					player->getId(),
				}
			);
		}
	}
}

void EquipmentManager::close()
{
	save();
}