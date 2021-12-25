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
	PveObject(uint32_t uid, eObjectType type, const Position& position, uint32_t linkedObjectId);

	virtual void tick(const std::shared_ptr<RoomSession>& roomSession);
	virtual void onEvent(const std::shared_ptr<RoomSession>& roomSession);

	eObjectType getType() const;
	Position getPosition() const;
protected:
	uint32_t m_uid;
	uint32_t m_linkedObjectId;
	eObjectType m_type;
	Position m_position;
};