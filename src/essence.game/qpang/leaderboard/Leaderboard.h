#pragma once

#include <cstdint>
#include <map>
#include <mutex>

class Leaderboard
{

public:
	struct Position {
		uint32_t rank;
		int32_t difference;
	};


	void refresh();
	Leaderboard::Position getPosition(uint32_t playerId);

private:
	std::map<uint32_t, Position> m_currentRanking;
	std::map<uint32_t, Position> m_oldRanking;

	std::mutex m_mx;
};
