#include "RoomSessionPveWaveManager.h"

#include "Game.h"
#include "RoomSession.h"

void RoomSessionPveWaveManager::initialize(const std::shared_ptr<RoomSession>& roomSession)
{
	m_roomSession = roomSession;
}

void RoomSessionPveWaveManager::tick()
{
	if (!m_isInitialized)
	{
		return;
	}

	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	const auto timeLeft = roomSession->getTimeLeftInSeconds();

	if (!m_waveIntervals.count(timeLeft))
	{
		// Time left does not occur in wave intervals so no npcs need to be spawned.
		return;
	}

	const auto currentWaveNumber = m_waveIntervals.at(timeLeft);

	if (!m_npcWaves.count(currentWaveNumber))
	{
		// Wave does not exist so therefor we can't spawn anything.
		return;
	}

	const auto& npcs = m_npcWaves.at(currentWaveNumber);

	for (const auto& npc : npcs)
	{
		roomSession->getNpcManager()->spawnNpc(std::make_shared<PveNpc>(npc));
	}
}

void RoomSessionPveWaveManager::initializeWaves()
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	m_npcWaves.clear();

	const auto mapId = roomSession->getPveRoundManager()->getMap();
	const auto npcWaveData = Game::instance()->getPveManager()->getNpcWaveData(mapId);

	for (const auto& [waveNumber, pveNpcWaveData] : npcWaveData)
	{
		for (const auto& data : pveNpcWaveData)
		{
			const auto [x, y, z] = data.initialPosition;

			const auto pathFinder = (y < 0)
				? roomSession->getUnderGroundPathfinder()
				: roomSession->getAboveGroundPathfinder();

			PathfinderCell spawnCell = {
				pathFinder->getCellX(x),
				pathFinder->getCellZ(z)
			};

			m_npcWaves[waveNumber].push_back(PveNpc(data, spawnCell));
		}
	}

	m_isInitialized = true;
}

void RoomSessionPveWaveManager::removeAll()
{
	m_npcWaves.clear();
}
