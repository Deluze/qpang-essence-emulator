#pragma once

#include <cstdint>
#include <memory>
#include <array>
#include <unordered_map>

#include "qpang/weapon/Weapon.h"

class RoomSessionPlayer;

class PlayerWeaponManager
{
public:
	PlayerWeaponManager();

	void initialize(const std::shared_ptr<RoomSessionPlayer>& player);

	Weapon getSelectedWeapon();

	void selectTagWeapon();
	void deselectTagWeapon();

	void reset();
	void reload(uint32_t seqId = 0);
	void shoot(uint32_t entityId);

	bool canReload();
	bool canShoot();

	bool hasWeapon(uint32_t weaponId) const;
	void switchWeapon(uint32_t weaponId, bool isReloadGlitchEnabled);
	bool isHoldingMelee() const;

	void refillWeapon(uint32_t weaponId);
	void refillCurrentWeapon();

	std::array<uint32_t, 4> getWeaponIds();
	std::array<Weapon, 4> getWeapons() const;

	bool hasEquippedMachineGun() const;

	uint64_t getEquippedMachineGunSeqId() const;

	void equipMachineGun(uint64_t seqId);
	void unequipMachineGun();

	void equipRainbowSkillCardWeapon(uint32_t weaponId);
	void unequipRainbowSkillCardWeapon();
private:
	std::array<Weapon, 4> m_weapons;
	uint8_t m_selectedWeaponIndex;

	std::unordered_map<uint32_t, std::pair<uint16_t, uint16_t>> m_defaultAmmo;

	std::weak_ptr<RoomSessionPlayer> m_player;

	Weapon m_currentRifleWeapon;
	uint8_t m_previousSelectedWeaponIndex;

	// Ground zero machine gun
	bool m_hasEquippedMachineGun;
	uint64_t m_equippedMachineGunSeqId;
};