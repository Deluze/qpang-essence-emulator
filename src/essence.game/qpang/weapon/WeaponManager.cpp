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
			getWeaponType(itemId),
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

WeaponType WeaponManager::getWeaponType(uint32_t weaponId)
{
	switch (weaponId)
	{
	case 1095172098:
	case 1095172100:
	case 1095172102:
	case 1095172103:
	case 1095172104:
	case 1095172105:
	case 1095172130:
	case 1095172131:
	case 1095761958:
	case 1095172147:
	case 1095172160:
	case 1095172097:
		return WeaponType::MELEE;
	case 1095237632:
	case 1095303169:
	case 1095761921:
	case 1095303170:
	case 1095303171:
	case 1095303172:
	case 1095303173:
	case 1095303177:
	case 1095303179:
	case 1095303180:
	case 1095303181:
	case 1095368720:
	case 1095303185:
	case 1095237671:
	case 1095237672:
	case 1095761961:
	case 1095303231:
	case 1095303233:
		return WeaponType::RIFLE;
	case 1095368704:
	case 1095368706:
	case 1095368707:
	case 1095368708:
	case 1095368709:
	case 1095368710:
	case 1095368711:
	case 1095368713:
	case 1095368716:
	case 1095368717:
	case 1095434253:
	case 1095368718:
	case 1095368719:
	case 1095368721:
	case 1095368734:
	case 1095368746:
	case 1095368756:
	case 1095368757:
	case 1095368758:
	case 1095368764:
	case 1095368766:
		return WeaponType::LAUNCHER;
	case 1095434240:
	case 1095499776:
	case 1095434241:
	case 1095499777:
	case 1095434242:
	case 1095499778:
	case 1095434243:
	case 1095499779:
	case 1095434244:
	case 1095434246:
	case 1095434249:
	case 1095434252:
	case 1095434255:
	case 1095434256:
	case 1095434257:
	case 1095434258:
	case 1095434277:
	case 1095434285:
	case 1095434287:
	case 1095434290:
	case 1095434295:
	case 1095434296:
	case 1095434297:
	case 1095434299:
	case 1095499837:
	default:
		return WeaponType::BOMB;
	}

	
}
