#pragma once

#include <memory>
#include "Position.h"
#include "PveNpc.h"
#include "PveObjectData.h"

class RoomSession;

class PveObject
{
public:
	virtual ~PveObject() = default;

	PveObject() = default;
	PveObject(uint32_t uid, eObjectType type, const Position& position, uint32_t linkedObjectId, uint16_t initialHealth);

	virtual void tick(const std::shared_ptr<RoomSession>& roomSession);

	void takeDamage(uint16_t damage);

	void onHitByNpc(const std::shared_ptr<RoomSession>& roomSession, const std::shared_ptr<PveNpc>& npc);

	virtual void onEvent(const std::shared_ptr<RoomSession>& roomSession);

	[[nodiscard]]
	eObjectType getType() const;

	[[nodiscard]]
	Position getPosition() const;

	[[nodiscard]]
	uint16_t getInitialHealth() const;

	[[nodiscard]]
	uint16_t getHealth() const;
protected:
	uint32_t m_uid{};
	uint32_t m_linkedObjectId{};
	eObjectType m_type;
	Position m_position{};

	uint16_t m_initialHealth{};
	uint16_t m_health{};
};