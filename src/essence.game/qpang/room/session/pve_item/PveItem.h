#pragma once

#include <memory>

#include "Position.h"

class RoomSession;

enum class eItemType : uint32_t
{
	GOLDEN_COIN = 1191182352,
	SILVER_COIN = 1191182353,
	BRONZE_COIN = 1191182354,
};

class PveItem final
{
public:
	PveItem() = default;
	PveItem(eItemType type, const Position& position);

	~PveItem() = default;

	void tick(const std::shared_ptr<RoomSession>& roomSession);

	/**
	 * \brief Sets the uid of the item.
	 * \param uid The uid of the item.
	 */
	void setUid(uint32_t uid);

	/**
	 * \brief Gets the uid of the item.
	 * \return The uid of the item.
	 */
	[[nodiscard]]
	uint32_t getUid() const;

	/**
	 * \brief Gets the type of the item.
	 * \return The type of the item.
	 */
	[[nodiscard]]
	eItemType getType() const;

	/**
	 * \brief Gets the position of the item.
	 * \return The position of the item.
	 */
	[[nodiscard]]
	Position getPosition() const;
protected:
	uint32_t m_uid{};
	eItemType m_type;
	Position m_position{};
};
