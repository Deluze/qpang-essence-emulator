#pragma once

#include <memory>
#include <array>
#include <cstdint>

#include "qpang/room/session/player/entity/PlayerBulletEntity.h"

class RoomSessionPlayer;

class PlayerEntityManager
{
public:
	void initialize(std::shared_ptr<RoomSessionPlayer> player);

	void shoot(uint32_t entityId);
	bool isValidShot(uint32_t entityId);
	void addKill(uint32_t entityId);

	void close();
private:
	std::array<PlayerBulletEntity, 20> m_validBullets;
	uint8_t m_bulletIndex = 0;
	
	std::weak_ptr<RoomSessionPlayer> m_player;
};