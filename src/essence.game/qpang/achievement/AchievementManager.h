#pragma once

#include <unordered_map>
#include <memory>

class RoomSessionPlayer;
class AchievementCondition;

enum class AchievementID : uint32_t
{
	WARRIOR = 11,
	FIGHTER = 12,
	SLAYER = 13,
	CHAMPION = 14,
	DESTROY_KING = 15,

	FRIEND = 21,
	LOVER = 22,
	ADDICT = 23,
	STOCKER = 24,
	MADNESS = 25,

	CHIEF = 51,
	HEAD = 52,
	LEADER = 53,
	BOSS = 54,
	HERO = 55,

	TUMBLER = 61,
	FAITHFUL = 62,
	SINGLE_MINDED = 63,
	IRON_WILL = 64,
	SUPERMAN = 65,

	TRICKY = 71,
	BRAIN = 72,
	ANALYST = 73,
	STAFF = 74,
	STRATEGIST = 75,

	NOVICE = 81,
	HARD_WORKING = 82,
	CHALLENGER = 83,	
	TRANSCENDENT = 84,
	ART_SEEKER = 85,

	SHOOTING_STAR = 91,
	DEMOLITIONER = 101,
	PITCHER = 111,
	SWORD_MASTER = 121,

	HEAD_COLLECTOR = 131,
	BIG_HEAD = 141,

	MANAGER = 161,
	HEAD_CHIEF = 162,
	PRESIDENT = 163,
	CHAIRMAN = 164,
	GODS_SON = 165,

	LUCKY_HAND = 171,
	BRASS_HAND = 172,
	SILVER_HAND = 173,
	GOLDEN_HAND = 174,
	GODS_HAND = 175,
	
	SPY = 181,
	SUSPECT = 191,
};

class AchievementManager
{
public:
	AchievementManager();

	void onPlayerFinish(const std::shared_ptr<RoomSessionPlayer>& session);
private:
	std::unordered_map<AchievementID, AchievementCondition*> m_achievements;
};