#pragma once

#include <memory>

class RoomSession;

class RoomSessionBossFightManager
{
public:
	/**
	 * \brief Initializes the boss fight manager with the room session.
	 * \param roomSession The room in session.
	 */
	void initialize(const std::shared_ptr<RoomSession>& roomSession);

	void tick();

	void handleBossDefeat();

private:
	std::weak_ptr<RoomSession> m_roomSession;

	bool m_hasSpawnedGoldenCoins = false;
};