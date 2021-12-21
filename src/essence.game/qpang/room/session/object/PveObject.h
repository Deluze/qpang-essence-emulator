#pragma once

#include <memory>
#include "Position.h"

enum class eObjectType : uint32_t {
	GATE = 1,
	DOOR = 2,
	MOVING_ESCALATOR = 3,
	STATIC_ESCALATOR = 4,
	SWITCH = 5,
	ESSENCE = 6
};

class RoomSession;

class PveObject
{
public:
	PveObject() = default;
	PveObject(eObjectType type, const Position& position);

	virtual ~PveObject() = default;

	void setUid(uint32_t uid);

	virtual void tick(std::shared_ptr<RoomSession> roomSession);

	eObjectType getType();
	Position getPosition();

protected:
	void move(std::shared_ptr<RoomSession> roomSession, const Position& from, const Position& to, int ticks);

	int m_moveTickCount;
	uint32_t m_uid{};
	eObjectType m_type;
	Position m_position{};
};