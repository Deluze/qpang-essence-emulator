#pragma once

#include <memory>

class RoomSession;

class RoomSessionPveRoundManager
{
public:
	/**
	 * \brief Initializes the pve round manager with the room session.
	 * \param roomSession The given room session.
	 */
	void initialize(const std::shared_ptr<RoomSession>& roomSession);
private:
	std::weak_ptr<RoomSession> m_roomSession;

	/*uint8_t m_currentRound = 0;*/
};