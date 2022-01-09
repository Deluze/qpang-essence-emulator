#pragma once

#include <memory>
#include <unordered_map>
#include <mutex>

#include "PveNpc.h"

class RoomSession;

class RoomSessionPveWaveManager
{
public:
	/**
	 * \brief Initializes the pve wave manager with the room session.
	 * \param roomSession The given room session.
	 */
	void initialize(const std::shared_ptr<RoomSession>& roomSession);

	/**
	 * \brief Ticks every tick.
	 */
	void tick();

	/**
	 * \brief Retrieves the waves + npc's from the PveManager.
	 */
	void initializeWaves();

	void removeAll();
private:
	std::weak_ptr<RoomSession> m_roomSession;

	std::unordered_map<uint32_t, std::vector<PveNpc>> m_npcWaves;
	std::unordered_map<uint32_t, uint32_t> m_waveIntervals
	{
		std::make_pair(595, 1),
		std::make_pair(540, 2),
		std::make_pair(480, 3),
		std::make_pair(420, 4),
		std::make_pair(360, 5),
		std::make_pair(300, 6),
		std::make_pair(240, 7),
		std::make_pair(180, 8),
		std::make_pair(120, 9),
		std::make_pair(60, 10)
	};

	bool m_isInitialized = false;
};
