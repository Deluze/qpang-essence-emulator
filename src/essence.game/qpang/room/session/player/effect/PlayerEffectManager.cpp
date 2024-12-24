#include "PlayerEffectManager.h"

#include <algorithm>

#include "qpang/room/session/RoomSession.h"
#include "qpang/room/session/player/RoomSessionPlayer.h"

#include "qpang/room/tnl/net_events/server/gc_hit.hpp"
#include "qpang/room/tnl/net_events/server/gc_game_state.hpp"
#include "qpang/room/tnl/net_events/server/gc_weapon.hpp"

void PlayerEffectManager::initialize(std::shared_ptr<RoomSessionPlayer> player)
{
	m_player = player;
	m_lastTick = time(NULL);
}

void PlayerEffectManager::tick()
{
	const auto currTime = time(NULL);

	if (currTime <= m_lastTick)
		return;

	m_lastTick = currTime;

	if (m_effects.empty())
		return;

	for (auto& effect : m_effects)
	{
		effect.weapon.effectDuration--;

		if (effect.weapon.effectId == 12 || effect.weapon.effectId == 13) // poison || fire
		{
			takeDamageFromEffect(effect);
		}
	}

	m_effects.erase(std::remove_if(m_effects.begin(), m_effects.end(),
		[&](const Effect& eff)
		{
			if (eff.weapon.effectDuration <= 0)
			{
				removeEffect(eff.weapon.effectId);
				return true;
			}

			return false;
		}
	), m_effects.end());
}

void PlayerEffectManager::addEffect(std::shared_ptr<RoomSessionPlayer> owner, const Weapon& weapon, uint32_t entityId)
{
	auto player = m_player.lock();

	if (player == nullptr)
		return;

	auto eff = Effect{
		entityId,
		weapon,
		owner,
	};

	m_effects.push_back(eff);
}

void PlayerEffectManager::clear()
{
	std::vector<uint8_t> alreadyClearedEffectIds;

	if (const auto player = m_player.lock(); player != nullptr)
	{
		for (const auto effect : m_effects)
		{
			if (std::find(alreadyClearedEffectIds.cbegin(), alreadyClearedEffectIds.cend(), effect.weapon.effectId) != alreadyClearedEffectIds.cend())
				continue;

			alreadyClearedEffectIds.push_back(effect.weapon.effectId);
			player->post(new GCWeapon(player->getPlayer()->getId(), 6, effect.weapon.effectId));
		}
	}

	m_effects.clear();
}

void PlayerEffectManager::removeEffect(uint8_t effectId)
{
	auto player = m_player.lock();

	if (player == nullptr)
		return;

	auto it = std::find_if(m_effects.begin(), m_effects.end(),
		[effectId](const Effect& eff)
		{
			return eff.weapon.effectId == effectId;
		}
	);

	size_t sameEffectsCount = std::count_if(m_effects.begin(), m_effects.end(),
		[effectId](const Effect& eff)
		{
			return eff.weapon.effectId == effectId && eff.weapon.effectDuration > 0;
		}
	);

	if (sameEffectsCount == 0) {
		// this was the last effect

		if (const auto roomSession = player->getRoomSession(); roomSession != nullptr)
		{
			roomSession->relayPlaying<GCWeapon>(player->getPlayer()->getId(), 6, effectId);
		}
	}
}

void PlayerEffectManager::takeDamageFromEffect(const Effect& effect)
{
	if (const auto player = m_player.lock(); player != nullptr)
	{
		if (player->isDead())
			return;

		player->takeHealth(5); // every effect that deals damage does 5 damage.

		if (const auto owner = effect.target.lock(); owner != nullptr)
		{
			if (const auto roomSession = player->getRoomSession(); roomSession != nullptr)
			{
				roomSession->relayPlaying<GCHit>(owner->getPlayer()->getId(), player->getPlayer()->getId(), 1, 0, 0, 0, 0, 0, 0, effect.entityId,
					1, 1, player->getHealth(), 5, effect.weapon.itemId, 1, 1, 1, owner->getStreak() + 1, 0, 0);
			}

			if (player->isDead())
			{
				if (const auto roomSession = owner->getRoomSession(); roomSession != nullptr)
				{
					roomSession->getGameMode()->onPlayerKill(owner, player, effect.weapon, 1);
					roomSession->relayPlaying<GCGameState>(player->getPlayer()->getId(), 17, effect.weapon.effectId, owner->getPlayer()->getId());
				}
			}
		}
	}
}
