#pragma once

#include <memory>

#include "Position.h"

class RoomSession;

enum class eNpcType : uint32_t
{
	EASY_VIOLENT_RABBIT = 1,
	EASY_NASTY_RABBIT = 2,
	EASY_CRAZY_RABBIT = 3,
	EASY_BLACK_CAT = 4,
	EASY_PATTERNED_CAT = 5,
	EASY_BLUE_MOUSE = 6,
	EASY_GRAY_MOUSE = 7,
	EASY_BROKEN_UFO = 8,
	EASY_VIOLENT_PLANT = 9,
	EASY_SPY_CAM = 10,
	EASY_CHESS_PAWN = 11,
	EASY_CHESS_BISHOP = 12,
	EASY_ABANDONED_DOLL = 13,
	NORMAL_VIOLENT_RABBIT = 14,
	NORMAL_NASTY_RABBIT = 16,
	NORMAL_CRAZY_RABBIT = 18,
	NORMAL_BLACK_CAT = 20,
	NORMAL_PATTERNED_CAT = 22,
	NORMAL_BLUE_MOUSE = 24,
	NORMAL_GRAY_MOUSE = 26,
	NORMAL_BROKEN_UFO = 28,
	NORMAL_VIOLENT_PLANT = 30,
	NORMAL_SPY_CAM = 32,
	NORMAL_CHESS_PAWN = 34,
	NORMAL_CHESS_BISHOP = 36,
	NORMAL_ABANDONED_DOLL = 38,
};

class PveNpc
{
public:
	/*static void tick(const std::shared_ptr<RoomSession>& roomSession);*/

	/**
	 * \brief Sets the uid of the npc.
	 * \param uid The uid of the npc.
	 */
	void setUid(uint32_t uid);

	/**
	 * \brief Subtracts the damage value from the health value.
	 * \param damage The damage to subtract from the health.
	 * \return The amount of damage taken.
	 */
	uint16_t takeDamage(uint16_t damage);

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

	/**
	 * \brief Gets the health of the npc.
	 * \return The health of the npc.
	 */
	[[nodiscard]]
	uint16_t getHealth() const;

	/**
	 * \brief Gets the initial spawn rotation of the npc.
	 * \return The initial spawn rotation.
	 */
	uint16_t getInitialSpawnRotation() const;

	/**
	 * \brief Whether or not the npc is dead.
	 * \return true if the npc is dead otherwise false.
	 */
	[[nodiscard]]
	bool isDead() const;
protected:
	PveNpc() = default;
	~PveNpc() = default;

	PveNpc(eNpcType type, const Position & position, uint16_t baseHealth, uint16_t initialSpawnRotation);

	uint32_t m_uid{};
	eNpcType m_type{};
	Position m_position{};

	uint16_t m_health{};

	uint16_t m_initialSpawnRotation;
};
