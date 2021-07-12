#include "WeaponManager.h"

#include <iostream>

void WeaponManager::initialize()
{
	auto stmt = DATABASE->prepare("SELECT * FROM weapons");
	auto res = stmt->fetch();

	if (!res->hasResults())
		return;

	std::lock_guard<std::mutex> lg(m_weaponMx);

	m_weapons.clear();

	std::cout << "Loading weapons.\n";

	while (res->hasNext())
	{
		uint32_t itemId = res->getInt("item_id");

		Weapon wp{
			itemId,
			res->getShort("damage"),
			res->getShort("clip_size"),
			res->getTiny("clip_amount"),
			res->getTiny("weight"),
			res->getTiny("effect_id"),
			res->getTiny("chance"),
			res->getTiny("duration"),
			getWeaponType(res->getInt("type")),
		};

		m_weapons[wp.itemId] = wp;

		res->next();
	}

	std::cout << "Loaded " << m_weapons.size() << " weapons.\n";
}

bool WeaponManager::canEquip(uint32_t weaponId, uint16_t characterId)
{
	auto weapon = get(weaponId);

	auto it = m_characterPower.find(characterId);

	if (it == m_characterPower.cend())
		return false;

	return weapon.weight <= it->second;
}

Weapon WeaponManager::get(uint32_t weaponId)
{
	std::lock_guard<std::mutex> lg(m_weaponMx);

	auto it = m_weapons.find(weaponId);

	if (it == m_weapons.cend())
		return {};

	return it->second;
}

// type: 0 = melee, 1 = rifle, 2 = launcher, 3 = bomb
WeaponType WeaponManager::getWeaponType(uint32_t type)
{

	switch (type)
	{
	case 0:
		return WeaponType::MELEE;
	case 1:
		return WeaponType::RIFLE;
	case 2:
		return WeaponType::LAUNCHER;
	case 3:
	// 999 is onbekend, maar die defaulten we dan naar BOMB.
	case 999:
	default:
		return WeaponType::BOMB;
	}
}
