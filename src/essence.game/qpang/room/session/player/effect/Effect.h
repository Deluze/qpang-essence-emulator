#pragma once

#include <cstdint>
#include <memory>

#include "qpang/weapon/Weapon.h"


class RoomSessionPlayer;

struct Effect
{
	uint32_t entityId = 0;
	Weapon weapon;
	std::weak_ptr<RoomSessionPlayer> target;
};