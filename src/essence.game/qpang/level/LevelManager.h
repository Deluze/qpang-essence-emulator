#pragma once

#include <map>
#include <memory>

class Player;
class RoomSessionPlayer;

class LevelManager
{
public:
	struct Level 
	{
		uint8_t level = 0;
		uint32_t minExperience = 0;
		uint32_t donReward = 0;
		uint32_t cashReward = 0;
		uint32_t coinReward = 0;
	};

	void initialize();

	Level getLevelForExperience(uint32_t experience, uint8_t cur = 0);
	void levelUp(std::shared_ptr<Player> player, uint8_t level);

	Level getLevelRewards(uint8_t level);

	void onPlayerFinish(const std::shared_ptr<RoomSessionPlayer>& player);
private:
	std::map<uint8_t, Level> m_levels;
};