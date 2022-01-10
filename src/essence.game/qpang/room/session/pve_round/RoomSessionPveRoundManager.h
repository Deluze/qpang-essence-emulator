#pragma once

#include <memory>

#include "RoomSessionPlayer.h"

enum class eRound : uint8_t
{
	CHESS_CASTLE_STAGE_1 = 0,
	CHESS_CASTLE_STAGE_2 = 1,
	CHESS_CASTLE_STAGE_3 = 2,
};

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
	void onStartNewRound(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer);

	/**
	 * \brief Ends the current round which causes the client to send a start new round packet/event.
	 */
	void endRound();

	void tick();

	void checkRoundZeroFinished();

	void checkRoundOneFinished() const;

	void checkRoundTwoFinished();

	[[nodiscard]]
	uint32_t getInitializedPlayerCount() const;

#pragma region Getters
	[[nodiscard]]
	uint8_t getMap() const;

	[[nodiscard]]
	eRound getCurrentRound() const;
#pragma endregion
private:
	void updatePathfinders() const;

	std::weak_ptr<RoomSession> m_roomSession;

	uint32_t m_initializedPlayerCount = 0;

	bool m_hasRoundEnded = false;

	uint8_t m_currentMap = 120;
	eRound m_currentRound = eRound::CHESS_CASTLE_STAGE_1;

	time_t m_endTime = NULL;
	bool m_endTimeHasBeenSet = false;
};