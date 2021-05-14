#pragma once

#include <memory>
#include <map>
#include <cstdint>
#include <mutex>
#include <vector>

#include "qpang/player/memo/Memo.h"

class Player;

class MemoManager
{
public:
	void initialize(std::shared_ptr<Player> player, uint32_t playerId);
	std::vector<Memo> list();
private:
	std::weak_ptr<Player> m_player;

	std::mutex m_memoMx;
	std::map<uint32_t, Memo> m_memos;
};