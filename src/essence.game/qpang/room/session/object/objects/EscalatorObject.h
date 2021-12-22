#pragma once

#include "PveObject.h"

struct EscalatorMoveInfo 
{
	// The starting position of the escalator
	Position m_startPos;

	// The ending position of the escalator
	Position m_endPos;

	// How long in seconds it should take to go from start to end, or from end to start
	int m_durationSeconds;

	// How long in seconds that the escalator should wait at the end pos before going back to the start pos, and vice-versa.
	int m_waitSeconds;
};

class EscalatorObject final : public PveObject
{
public:
	EscalatorObject(const EscalatorMoveInfo& moveInfo);

	void tick(const std::shared_ptr<RoomSession>& roomSession) override;
private:
	EscalatorMoveInfo m_moveInfo;
};