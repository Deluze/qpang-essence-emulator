#pragma once

#include <mutex>
#include <vector>

class Player;

class AchievementContainer
{
public:
	void initialize(const uint32_t playerId);
	void unlock(const uint32_t achievementId);

	void resetRecent();

	std::vector<uint32_t> list();
	std::vector<uint32_t> listRecent();
private:
	std::vector<uint32_t> m_achievements;
	std::vector<uint32_t> m_recentAchievements;

	uint32_t m_playerId;

	std::mutex m_mx;
};