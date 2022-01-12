#include "RoomSessionBossFightManager.h"

#include "Maps.h"
#include "PveBossNpc.h"
#include "PveItem.h"
#include "RoomSession.h"

void RoomSessionBossFightManager::initialize(const std::shared_ptr<RoomSession>& roomSession)
{
	m_roomSession = roomSession;
}

void RoomSessionBossFightManager::tick()
{
	handleBossDefeat();
}

void RoomSessionBossFightManager::handleBossDefeat()
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	const auto& bossNpc = roomSession->getNpcManager()->findNpcByUid(1);

	if (bossNpc == nullptr)
	{
		return;
	}

	if (bossNpc->isDead() && !m_hasSpawnedGoldenCoins)
	{
		m_hasSpawnedGoldenCoins = true;

		const auto cellWidth = Maps::pveStage3MapInfo.m_cellWidth;

		const float posX = 5.0f * cellWidth;
		// ReSharper disable once CppTooWideScope
		const float posZ = 5.0f * cellWidth;

		for (float x = -posX; x <= posX; x += cellWidth * 2.0f)
		{
			for (float z = -posZ; z <= posZ; z += cellWidth * 2.0f)
			{
				const auto goldCoin = PveItem(static_cast<uint32_t>(eItemId::GOLDEN_COIN), Position{ x, 0, z });

				roomSession->getPveItemManager()->spawnItem(std::make_shared<PveItem>(goldCoin));
			}
		}
	}
}
