#include "AchievementContainer.h"

#include "qpang/Game.h"
#include "qpang/player/Player.h"

void AchievementContainer::initialize(const uint32_t playerId)
{
	m_playerId = playerId;

	auto stmt = DATABASE->prepare("SELECT * FROM player_achievements WHERE player_id = ?");
	stmt->bindInteger(playerId);
	auto res = stmt->fetch();

	std::lock_guard<std::mutex> l(m_mx);
	
	while (res->hasNext())
	{
		const auto achievementId = res->getInt("achievement_id");

		m_achievements.push_back(achievementId);

		res->next();
	}
}

void AchievementContainer::unlock(const uint32_t achievementId)
{
	std::lock_guard<std::mutex> l(m_mx);
	
	auto it = std::find(m_achievements.cbegin(), m_achievements.cend(), achievementId);

	if (it != m_achievements.cend())
		return;

	DATABASE_DISPATCHER->dispatch("INSERT INTO player_achievements (player_id, achievement_id) VALUES (?, ?)", { m_playerId, achievementId });

	m_achievements.push_back(achievementId);
	m_recentAchievements.push_back(achievementId);
}

void AchievementContainer::resetRecent()
{
	m_recentAchievements.clear();
}

std::vector<uint32_t> AchievementContainer::list()
{
	return m_achievements;
}

std::vector<uint32_t> AchievementContainer::listRecent()
{
	return m_recentAchievements;
}
