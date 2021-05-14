#include "MemoManager.h"

#include "core/Emulator.h"
#include "core/database/Database.h"

void MemoManager::initialize(std::shared_ptr<Player> player, uint32_t playerId)
{
	m_player = player;

	const auto stmt = DATABASE->prepare("SELECT memos.*, players.name FROM memos JOIN players ON players.id = memos.sender_id WHERE receiver_id = ?");
	stmt->bindInteger(playerId);
	const auto res = stmt->fetch();

	while (res->hasNext())
	{
		Memo memo{
			res->getBigInt("id"),
			res->getInt("sender_id"),
			res->getString("name"),
			res->getString("message"),
			res->getFlag("opened"),
			static_cast<time_t>(res->getInt("created"))
		};

		m_memos[memo.id] = memo;

		res->next();
	}
}

std::vector<Memo> MemoManager::list()
{
	std::lock_guard<std::mutex> lg(m_memoMx);

	std::vector<Memo> memos;

	for (const auto& [id, memo] : m_memos)
		memos.push_back(memo);

	return memos;
}
