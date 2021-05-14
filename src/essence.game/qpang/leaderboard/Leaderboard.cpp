#include "Leaderboard.h"

#include "core/Emulator.h"
#include "core/database/Database.h"

#include "qpang/Game.h"
#include "packets/lobby/outgoing/player/UpdatePlayerRanking.h"

void Leaderboard::refresh()
{
	std::cout << "Loading leaderboard... ";

	std::lock_guard<std::mutex> g(m_mx);

	if (!m_currentRanking.empty())
		m_oldRanking = m_currentRanking;

	auto res = DATABASE->prepare("SELECT id, experience FROM players ORDER BY experience DESC")->fetch();

	int32_t rank = 1;

	while (res->hasNext())
	{
		const auto playerId = res->getInt("id");

		Position pos{};
		pos.rank = rank;

		auto oldPos = m_oldRanking.find(playerId);
		if (oldPos != m_oldRanking.cend())
			pos.difference = oldPos->second.rank - rank;

		m_currentRanking[playerId] = pos;

		const auto player = Game::instance()->getOnlinePlayer(playerId);

		if (player != nullptr)
			player->send(UpdatePlayerRanking(pos));

		rank++;
		res->next();
	}

	std::cout << "loaded!" << '\n';
}

Leaderboard::Position Leaderboard::getPosition(uint32_t playerId)
{
	std::lock_guard<std::mutex> g(m_mx);

	auto it = m_currentRanking.find(playerId);

	if (it == m_currentRanking.cend())
		return {};
	
	return it->second;
}
