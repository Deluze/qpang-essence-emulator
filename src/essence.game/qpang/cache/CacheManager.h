#pragma once

#include "qpang/cache/player/PlayerCacheManager.h"

class CacheManager
{
public:
	void clear();

	PlayerCacheManager* getPlayerCacheManager();
private:
	PlayerCacheManager m_playerCacheManager;
};