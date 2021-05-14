#include "CacheManager.h"

void CacheManager::clear()
{
	m_playerCacheManager.clear();
}

PlayerCacheManager* CacheManager::getPlayerCacheManager()
{
	return &m_playerCacheManager;
}
