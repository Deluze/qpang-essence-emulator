#pragma once

#include <ctime>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <memory>

#include "qpang/room/session/player/effect/Effect.h"

class PlayerEffectManager
{
public:
	void initialize(std::shared_ptr<RoomSessionPlayer> player);
	void tick();

	// duration = in seconds
	void addEffect(std::shared_ptr<RoomSessionPlayer> owner, const Weapon& weapon, uint32_t entityId);

	void clear();

private:

	void removeEffect(uint8_t effectId);
	void takeDamageFromEffect(const Effect& effect);

	time_t m_lastTick;

	std::vector<Effect> m_effects;

	std::weak_ptr<RoomSessionPlayer> m_player;
};