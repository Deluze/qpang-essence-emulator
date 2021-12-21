#pragma once

#include <memory>
#include "Position.h"

enum class eObjectType : uint32_t {
	SMALL_GATE = 1,
	BIG_GATE = 2,
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
	void setShouldMove(bool shouldMove);

	virtual void tick(std::shared_ptr<RoomSession> roomSession);
	virtual void onEvent(std::shared_ptr<RoomSession> roomSession);

	eObjectType getType();
	Position getPosition();

protected:
	void move(std::shared_ptr<RoomSession> roomSession, const Position& from, const Position& to, int ticks);

	bool m_shouldMove;
	int m_moveTickCount;
	uint32_t m_uid{};
	eObjectType m_type;
	Position m_position{};
};