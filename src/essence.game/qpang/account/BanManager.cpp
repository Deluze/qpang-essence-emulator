#include "BanManager.h"

#include <time.h>

#include "core/Emulator.h"
#include "core/database/Database.h"

bool BanManager::isBanned(uint32_t userId)
{
	Statement::Ptr stmt = DATABASE->prepare("SELECT * FROM user_bans WHERE user_id = ? AND timestamp_unban > ? ORDER BY id DESC LIMIT 1");

	stmt->bindInteger(userId);
	stmt->bindLong(static_cast<uint64_t>(time(NULL)));

	StatementResult::Ptr res = stmt->fetch();

	return res->hasResults();
}