#include "LevelManager.h"

#include <iostream>

#include "core/Emulator.h"
#include "core/database/Database.h"

#include "qpang/room/session/player/RoomSessionPlayer.h"
#include "qpang/player/Player.h"

void LevelManager::initialize()
{
	auto stmt = DATABASE->prepare("SELECT * FROM levels");
	auto res = stmt->fetch();

	while (res->hasNext())
	{
		const auto level = res->getTiny("level");
		const auto experience = res->getInt("experience");
		const auto donReward = res->getInt("don_reward");
		const auto cashReward = res->getInt("cash_reward");
		const auto coinReward = res->getInt("coin_reward");

		m_levels[level] = {
			level,
			experience,
			donReward,
			cashReward,
			coinReward
		};

		res->next();
	}
}

LevelManager::Level LevelManager::getLevelForExperience(uint32_t experience, uint8_t currentLevel)
{
	for (const auto& level : m_levels)
	{
		if (level.second.minExperience < experience)
			continue;
		else {
			if (level.first == 1)
				return level.second;

			return getLevelRewards(level.first - 1);
		}
	}

	return Level{ 0, 0, 0, 0, 0 };
}

void LevelManager::levelUp(std::shared_ptr<Player> player, uint8_t level)
{

}

LevelManager::Level LevelManager::getLevelRewards(uint8_t level)
{
	assert(level > 0 && level <= 45);

	return m_levels[level];
}

void LevelManager::onPlayerFinish(const std::shared_ptr<RoomSessionPlayer>& session)
{
	auto player = session->getPlayer();

	const auto currentLevel = player->getLevel();
	const auto targetLevel = getLevelForExperience(player->getExperience());

	if (targetLevel.level <= currentLevel)
		return;

	for (size_t i = currentLevel + 1; i <= targetLevel.level; i++)
	{
		const auto rewards = getLevelRewards(i);

		player->addDon(rewards.donReward);
		//player->addCash(rewards.cashReward);
	}

	player->setLevel(targetLevel.level);
}