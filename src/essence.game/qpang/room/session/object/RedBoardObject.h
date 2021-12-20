#pragma once

#include "PveObject.h"

enum class eRedBoardActionId {
	STAGE0_PIT1_BOARD1
};

class RedBoardObject : public PveObject
{
public:
	RedBoardObject(eRedBoardActionId actionId, const Position& position);

	void tick(std::shared_ptr<RoomSession> roomSession) override;
private:
	eRedBoardActionId m_actionId;
	int m_tickCount = 0;
};