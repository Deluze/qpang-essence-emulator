#pragma once

#include <cstdint>

enum class WeaponType : uint8_t
{
	MELEE,
	RIFLE,
	LAUNCHER,
	BOMB,
};

struct Weapon
{
	uint32_t itemId = 0;
	uint16_t damage = 0;
	uint16_t clipSize = 0;
	uint8_t clipCount = 0;
	uint8_t weight = 99;
	uint8_t effectId = 0;
	uint8_t effectChance = 0;
	uint8_t effectDuration = 0; // in seconds
	WeaponType weaponType = WeaponType::MELEE;
};