#pragma once

#include "PveObject.h"

class MoveableObject : public PveObject
{
public:
	MoveableObject(uint32_t uid, eObjectType type, const Position& position, const Position& endPosition, int moveDuration, int moveWait, uint32_t linkedObjectId);

	void tick(const std::shared_ptr<RoomSession>& roomSession) override;
private:
	void move(const std::shared_ptr<RoomSession>& roomSession, const Position& from, const Position& to, int ticks, int waitTicks);

	Position m_endPosition;
	int m_moveDuration;
	int m_moveWait;
	int m_moveTickCount;
};