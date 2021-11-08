#include "WeaponManager.h"

#include <iostream>

void WeaponManager::initialize()
{
	const auto statement = DATABASE->prepare("SELECT * FROM weapons");
	const auto result = statement->fetch();

	if (!result->hasResults())
	{
		return;
	}

	std::lock_guard lg(m_weaponMx);

	m_weapons.clear();

	std::cout << "Loading weapons.\n";

	while (result->hasNext())
	{
		const uint32_t itemId = result->getInt("item_id");

		Weapon weapon{
			itemId,
			result->getShort("damage"),
			result->getShort("clip_size"),
			result->getTiny("clip_amount"),
			result->getTiny("weight"),
			result->getTiny("effect_id"),
			result->getTiny("chance"),
			result->getTiny("duration"),
			getWeaponType(result->getInt("type")),
		};

		m_weapons[weapon.itemId] = weapon;

		result->next();
	}

	std::cout << "Loaded " << m_weapons.size() << " weapons.\n";
}

bool WeaponManager::canEquip(const uint32_t weaponId, const uint16_t characterId)
{
	const auto weapon = get(weaponId);

	const auto it = m_characterPower.find(characterId);

	if (it == m_characterPower.cend())
	{
		return false;
	}

	return weapon.weight <= it->second;
}

Weapon WeaponManager::get(const uint32_t weaponId)
{
	std::lock_guard lg(m_weaponMx);

	const auto it = m_weapons.find(weaponId);

	if (it == m_weapons.cend())
	{
		return {};
	}

	return it->second;
}

// type: 0 = melee, 1 = rifle, 2 = launcher, 3 = bomb
WeaponType WeaponManager::getWeaponType(const uint32_t weaponType)
{
	switch (weaponType)
	{
	case 0:
		return MELEE;
	case 1:
		return RIFLE;
	case 2:
		return LAUNCHER;
	case 3:
	// 999 = unknown, so we default to bomb.
	case 999:
	default:
		return BOMB;
	}
}
