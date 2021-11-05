#include "PlayerWeaponManager.h"

#include "qpang/Game.h"
#include "qpang/room/session/RoomSession.h"

#include "qpang/room/tnl/net_events/client/cg_weapon.hpp"

#include "qpang/room/tnl/net_events/server/gc_weapon.hpp"
#include "qpang/room/tnl/net_events/server/gc_respawn.hpp"
#include "qpang/room/tnl/net_events/server/gc_game_item.hpp"
#include "qpang/room/tnl/net_events/server/gc_essence.hpp"

constexpr auto RIFLE_INDEX = 0;

PlayerWeaponManager::PlayerWeaponManager() :
	m_selectedWeaponIndex(0),
	m_previousSelectedWeaponIndex(0),
	m_hasEquippedMachineGun(false),
	m_equippedMachineGunSeqId(0)
{
}

void PlayerWeaponManager::initialize(const std::shared_ptr<RoomSessionPlayer>& player)
{
	m_player = player;

	auto* equipmentManager = player->getPlayer()->getEquipmentManager();

	const auto itemIds = equipmentManager->getWeaponItemIdsByCharacter(player->getCharacter());
	const auto isMeleeOnly = player->getRoomSession()->getRoom()->isMeleeOnly();

	for (int i = 0; i < itemIds.size(); i++)
	{
		const auto weapon = Game::instance()->getWeaponManager()->get(itemIds[i]);

		if (isMeleeOnly)
		{
			// melee weapon
			if (i == 3)
			{
				m_weapons[3] = weapon;
			}
		}
		else
		{
			m_weapons[i] = weapon;
		}

		auto& [first, second] = m_defaultAmmo[weapon.itemId];

		first = weapon.clipCount + equipmentManager->getExtraAmmoForWeaponIndex(i);
		second = weapon.clipSize;
	}

	m_selectedWeaponIndex = 3;
}

Weapon PlayerWeaponManager::getSelectedWeapon()
{
	return m_weapons[m_selectedWeaponIndex];
}

void PlayerWeaponManager::selectTagWeapon()
{
	m_previousSelectedWeaponIndex = m_selectedWeaponIndex;

	m_selectedWeaponIndex = RIFLE_INDEX;
	m_currentRifleWeapon = m_weapons[m_selectedWeaponIndex];

	const auto chainLightWeapon = Game::instance()->getWeaponManager()->get(WEAPON_PREY_CHAIN_LIGHT);

	m_weapons[RIFLE_INDEX] = chainLightWeapon;

	auto& [first, second] = m_defaultAmmo[chainLightWeapon.itemId];

	first = chainLightWeapon.clipCount;
	second = chainLightWeapon.clipSize;

	refillCurrentWeapon();

	switchWeapon(m_weapons[m_selectedWeaponIndex].itemId, false);
}

void PlayerWeaponManager::deselectTagWeapon()
{
	m_weapons[RIFLE_INDEX] = m_currentRifleWeapon;
	m_selectedWeaponIndex = m_previousSelectedWeaponIndex;

	switchWeapon(m_weapons[m_selectedWeaponIndex].itemId, false);

	reset();
}

void PlayerWeaponManager::reset()
{
	const auto player = m_player.lock();

	if (player == nullptr)
		return;

	// ReSharper disable once CppUseStructuredBinding
	for (auto& weapon : m_weapons)
	{
		const auto [first, second] = m_defaultAmmo[weapon.itemId];

		weapon.clipCount = first;
		weapon.clipSize = second;

		player->post(new GCGameItem(14, { {1191182337, 1} }, weapon.itemId));
	}
}

void PlayerWeaponManager::reload(const uint32_t seqId)
{
	m_weapons[m_selectedWeaponIndex].clipCount--;
	m_weapons[m_selectedWeaponIndex].clipSize = m_defaultAmmo[getSelectedWeapon().itemId].second;

	if (const auto player = m_player.lock(); player != nullptr)
		player->getRoomSession()->relayPlaying<GCWeapon>(player->getPlayer()->getId(), 3, m_weapons[m_selectedWeaponIndex].itemId, seqId);
}

void PlayerWeaponManager::shoot(uint32_t entityId)
{
	m_weapons[m_selectedWeaponIndex].clipSize--;
}

bool PlayerWeaponManager::canReload()
{
	return m_weapons[m_selectedWeaponIndex].clipCount > 0;
}

bool PlayerWeaponManager::canShoot()
{
	if (isHoldingMelee())
	{
		return true;
	}

	return m_weapons[m_selectedWeaponIndex].clipSize > 0;
}

bool PlayerWeaponManager::hasWeapon(uint32_t weaponId) const
{
	return std::find_if(m_weapons.cbegin(), m_weapons.cend(),
		[weaponId](const Weapon& weapon)
		{
			return weapon.itemId == weaponId;
		}
	) != m_weapons.cend();
}

void PlayerWeaponManager::switchWeapon(uint32_t weaponId, bool isReloadGlitchEnabled)
{
	if (!isReloadGlitchEnabled)
	{
		if (m_weapons[m_selectedWeaponIndex].itemId == weaponId)
		{
			// already chose this weapon
			return;
		}
	}

	for (size_t i = 0; i < m_weapons.size(); i++)
	{
		if (m_weapons[i].itemId == weaponId)
		{
			m_selectedWeaponIndex = i;
			break;
		}
	}

	if (const auto player = m_player.lock(); player != nullptr)
	{
		player->getRoomSession()->relayPlaying<GCWeapon>(player->getPlayer()->getId(), 0, m_weapons[m_selectedWeaponIndex].itemId, 0);
		player->post(new GCWeapon(player->getPlayer()->getId(), 5, m_weapons[m_selectedWeaponIndex].itemId, 0));
	}
}

void PlayerWeaponManager::refillWeapon(const uint32_t weaponId)
{
	const auto [first, second] = m_defaultAmmo[weaponId];

	m_weapons[m_selectedWeaponIndex].clipCount = first;
	m_weapons[m_selectedWeaponIndex].clipSize = second;

	if (const auto player = m_player.lock(); player != nullptr)
	{
		player->post(new GCGameItem(
			14,
			std::vector({ GCGameItem::Item{1191182337, 0x01, 0x00, 0x00, 0x00} }),
			weaponId
		));
	}
}

void PlayerWeaponManager::refillCurrentWeapon()
{
	const auto itemId = getSelectedWeapon().itemId;

	refillWeapon(itemId);
}

std::array<uint32_t, 4> PlayerWeaponManager::getWeaponIds()
{
	std::array<uint32_t, 4> weaponIds{};

	for (size_t i = 0; i < m_weapons.size(); i++)
	{
		weaponIds[i] = m_weapons[i].itemId;
	}

	return weaponIds;
}

std::array<Weapon, 4> PlayerWeaponManager::getWeapons() const
{
	return m_weapons;
}

bool PlayerWeaponManager::hasEquippedMachineGun() const
{
	return m_hasEquippedMachineGun;
}

uint64_t PlayerWeaponManager::getEquippedMachineGunSeqId() const
{
	return m_equippedMachineGunSeqId;
}

void PlayerWeaponManager::equipMachineGun(const uint64_t seqId)
{
	// If the player attempts to equip the machine gun whilst already equipping a machine gun, disallow it.
	if (m_hasEquippedMachineGun)
	{
		return;
	}

	// Check if the seqId is a valid sequence id.
	if (seqId < 1 || seqId > 4)
	{
		return;
	}

	// TODO: Perhaps check if the player is "in range" of the machine gun (by seq id).
	// TODO: Check if the machine gun with the seq id is already taken by another player.

	if (const auto player = m_player.lock(); player != nullptr)
	{
		if (player->getSkillManager()->hasActiveSkillCard())
		{
			return;
		}

		// Disallow the machine gun in public enemy mode for now.
		if (player->getRoomSession()->getGameMode()->isPublicEnemyMode())
		{
			player->getPlayer()->broadcast(u"The machine gun is disabled in the public enemy gamemode.");

			return;
		}

		// Let client know the player is shooting with ground zero gun.
		player->getRoomSession()->relayPlaying<GCWeapon>(player->getPlayer()->getId(), CGWeapon::CMD::EQUIP_MACHINE_GUN, WEAPON_MACHINE_GUN, seqId);
		// Allow client to shoot.
		player->getRoomSession()->relayPlaying<GCWeapon>(player->getPlayer()->getId(), CGWeapon::CMD::ENABLE_SHOOTING, WEAPON_MACHINE_GUN, seqId);

		// Save the currently selected weapon index.
		m_previousSelectedWeaponIndex = m_selectedWeaponIndex;
		// Set the currently selected weapon index to gun.
		m_selectedWeaponIndex = 0;
		// Set the current gun weapon to the current weapon.
		m_currentRifleWeapon = m_weapons[m_selectedWeaponIndex];

		const auto machineGunWeapon = Game::instance()->getWeaponManager()->get(WEAPON_MACHINE_GUN);

		m_weapons[0] = machineGunWeapon;

		auto& [first, second] = m_defaultAmmo[machineGunWeapon.itemId];

		first = machineGunWeapon.clipCount;
		second = machineGunWeapon.clipSize;

		refillCurrentWeapon();
		switchWeapon(machineGunWeapon.itemId, false);

		m_hasEquippedMachineGun = true;
		m_equippedMachineGunSeqId = seqId;
	}
}

void PlayerWeaponManager::unequipMachineGun()
{
	// If the player does not have a machine gun equipped, do not allow them to unequip one.
	if (!m_hasEquippedMachineGun)
	{
		return;
	}

	if (const auto player = m_player.lock(); player != nullptr)
	{
		// Disallow the machine gun in public enemy mode for now.
		if (player->getRoomSession()->getGameMode()->isPublicEnemyMode())
		{
			player->getPlayer()->broadcast(u"The machine gun is disabled in the public enemy gamemode.");

			return;
		}

		player->getRoomSession()->relayPlaying<GCWeapon>(player->getPlayer()->getId(), CGWeapon::CMD::UNEQUIP_MACHINE_GUN, WEAPON_MACHINE_GUN, m_equippedMachineGunSeqId);

		m_weapons[0] = m_currentRifleWeapon;
		m_selectedWeaponIndex = m_previousSelectedWeaponIndex;

		switchWeapon(m_weapons[m_selectedWeaponIndex].itemId, false);

		m_hasEquippedMachineGun = false;
		m_equippedMachineGunSeqId = 0;
	}
}

void PlayerWeaponManager::equipRainbowSkillCardWeapon(uint32_t weaponId)
{
	m_previousSelectedWeaponIndex = m_selectedWeaponIndex;

	m_selectedWeaponIndex = RIFLE_INDEX;
	m_currentRifleWeapon = m_weapons[m_selectedWeaponIndex];

	const auto weapon = Game::instance()->getWeaponManager()->get(weaponId);

	m_weapons[RIFLE_INDEX] = weapon;

	auto& [first, second] = m_defaultAmmo[weapon.itemId];

	first = weapon.clipCount;
	second = weapon.clipSize;

	refillCurrentWeapon();

	switchWeapon(m_weapons[m_selectedWeaponIndex].itemId, false);
}

void PlayerWeaponManager::unequipRainbowSkillCardWeapon()
{
	m_weapons[RIFLE_INDEX] = m_currentRifleWeapon;
	m_selectedWeaponIndex = m_previousSelectedWeaponIndex;

	switchWeapon(m_weapons[m_selectedWeaponIndex].itemId, false);

	reset();
}

bool PlayerWeaponManager::isHoldingMelee() const
{
	return m_selectedWeaponIndex == 3;
}

