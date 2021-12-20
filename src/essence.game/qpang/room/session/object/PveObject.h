#pragma once

#include <memory>
#include "Position.h"

enum class eObjectType : uint32_t {
	GATE = 1,
	DOOR = 2,
	MOVING_RED_BOARD = 3,
	RED_BOARD = 4,
	SWITCH = 5,
	ESSENCE = 6
};

class RoomSession;

class PveObject
{
public:
	PveObject() {}
	PveObject(eObjectType type, const Position& position);

	void setUid(uint32_t uid);

	virtual void tick(std::shared_ptr<RoomSession> roomSession);

	eObjectType getType();
	Position getPosition();

protected:
	uint32_t m_uid;
	eObjectType m_type;
	Position m_position;
};