#pragma once

#include "qpang/room/session/game_item/GameItem.h"

class AmmoClip : public GameItem
{
public:
	uint32_t onPickUp(std::shared_ptr<RoomSessionPlayer> session) override
	{
		const auto weaponManager = session->getWeaponManager();
		weaponManager->refillCurrentWeapon();

		return 0;
	}
};