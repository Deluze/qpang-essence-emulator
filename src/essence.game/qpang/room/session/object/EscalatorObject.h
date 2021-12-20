#pragma once

#include "PveObject.h"

enum class eRedBoardActionId {
	STAGE0_PIT1_ESCALATOR1,
	STAGE0_PIT2_ESCALATOR1
};

class EscalatorObject : public PveObject
{
public:
	EscalatorObject(eRedBoardActionId actionId, const Position& position);

	void tick(std::shared_ptr<RoomSession> roomSession) override;
private:
	void move(std::shared_ptr<RoomSession> roomSession, const Position& from, const Position& to, int ticks);

	eRedBoardActionId m_actionId;
	int m_tickCount = 0;
};