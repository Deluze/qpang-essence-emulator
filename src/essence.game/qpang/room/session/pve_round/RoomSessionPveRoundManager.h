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

	/**
	 * \brief Called from PlayerVsEnvironment::onStart (when the room session starts).
	 */
	void onStart() const;

	/**
	 * \brief Handles the CGPvERoundStart event by relaying the CGPvENewRound event.
	 */
	void onStartNewRound();

	/**
	 * \brief Ends the current round which causes the client to send a start new round packet/event.
	 */
	void endRound();

	void tick();

	void checkRoundZeroFinished();

#pragma region Getters
	uint8_t getMap() const;
#pragma endregion
private:
	void updatePathfinders() const;

	std::weak_ptr<RoomSession> m_roomSession;

	bool m_hasRoundEnded = false;

	uint8_t m_currentRound = 0;
	uint8_t m_currentMap = 120;
};