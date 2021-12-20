#pragma once

#include <memory>

#include "Position.h"

class RoomSession;

enum class eNpcType : uint32_t
{
	VIOLENT_RABBIT = 1,
	NASTY_RABBIT = 2,
	CRAZY_RABBIT = 3,
	BLACK_CAT = 4,
	PATTERNED_CAT = 5,
	BLUE_MOUSE = 6,
	GRAY_MOUSE = 7,
	BROKEN_UFO = 8,
	VIOLENT_PLANT = 9,
	SPY_CAM = 10,
	CHESS_PAWN = 11,
	CHESS_BISHOP = 12,
	ABANDONED_DOLL = 13,
};

class PveNpc final
{
public:
	PveNpc() = default;

	PveNpc(eNpcType type, const Position& position);

	~PveNpc() = default;

	void tick(const std::shared_ptr<RoomSession>& roomSession);

	/**
	 * \brief Sets the uid of the npc.
	 * \param uid The uid of the npc.
	 */
	void setUid(uint32_t uid);

	/**
	 * \brief Gets the uid of the npc.
	 * \return The uid of the npc.
	 */
	[[nodiscard]]
	uint32_t getUid() const;

	/**
	 * \brief Gets the type of the npc.
	 * \return The type of the npc.
	 */
	[[nodiscard]]
	eNpcType getType() const;

	/**
	 * \brief Gets the position of the npc.
	 * \return The position of the npc.
	 */
	[[nodiscard]]
	Position getPosition() const;
protected:
	uint32_t m_uid{};
	eNpcType m_type;
	Position m_position{};
};
