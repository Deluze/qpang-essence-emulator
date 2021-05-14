#include "PlayerCacheManager.h"

#include "utils/StringConverter.h"
#include "core/Emulator.h"
#include "core/database/Database.h"

Player::Ptr PlayerCacheManager::tryGet(uint32_t playerId)
{
	std::lock_guard<std::mutex> lg(m_cacheMx);

	const auto it = m_playersById.find(playerId);

	return it != m_playersById.cend() ? (*it).second : nullptr;
}

Player::Ptr PlayerCacheManager::tryGet(const std::u16string& nickname)
{
	std::lock_guard<std::mutex> lg(m_cacheMx);

	const auto it = m_playersByName.find(StringConverter::ToLowerCase(nickname));

	return it != m_playersByName.cend() ? (*it).second : nullptr;
}

Player::Ptr PlayerCacheManager::cache(uint32_t playerId)
{
	const auto player = std::make_shared<Player>(playerId);
	player->initialize();

	if (!player->exists())
		return nullptr;

	std::lock_guard<std::mutex> lg(m_cacheMx);

	m_playersById[player->getId()] = player;
	m_playersByName[StringConverter::ToLowerCase(player->getName())] = player;

	return player;
}

Player::Ptr PlayerCacheManager::cache(const std::u16string& nickname)
{
	const auto stmt = DATABASE->prepare("SELECT id FROM players WHERE name = ?");
	stmt->bindString(nickname);
	const auto res = stmt->fetch();

	if (!res->hasResults())
		return nullptr;

	auto playerId = res->getInt("id");
	return cache(playerId);
}

void PlayerCacheManager::cache(Player::Ptr player)
{
	assert(player != nullptr);
	assert(player->exists());

	std::lock_guard<std::mutex> lg(m_cacheMx);

	m_playersById[player->getId()] = player;
	m_playersByName[StringConverter::ToLowerCase(player->getName())] = player;
}

void PlayerCacheManager::invalidate(uint32_t playerId)
{
	auto player = tryGet(playerId);

	if (player == nullptr)
		return;

	std::lock_guard<std::mutex> lg(m_cacheMx);

	m_playersById.erase(player->getId());
	m_playersByName.erase(StringConverter::ToLowerCase(player->getName()));
}

void PlayerCacheManager::clear()
{
	std::lock_guard<std::mutex> lg(m_cacheMx);

	m_playersById.clear();
	m_playersByName.clear();
}
