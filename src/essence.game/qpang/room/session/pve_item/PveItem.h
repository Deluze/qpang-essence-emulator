#pragma once

#include <memory>

#include "Position.h"
#include "RoomSessionPlayer.h"

class RoomSession;

enum class eItemType : uint32_t
{
	NONE = 0,
	AMMO_CLIP = 1191182337,
	RED_MEDKIT = 1191182338,
	GOLDEN_COIN = 1191182352,
	SILVER_COIN = 1191182353,
	BRONZE_COIN = 1191182354
};

class PveItem final
{
public:
	PveItem() = default;
	PveItem(uint32_t itemId, const Position& position);

	~PveItem() = default;

	void tick(const std::shared_ptr<RoomSession>& roomSession);

	/**
	 * \brief Relays the item init event.
	 * \param roomSession The room in session.
	 */
	void spawn(const std::shared_ptr<RoomSession>& roomSession) const;
	void spawn(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer) const;
	
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
	 * \brief Gets the item id.
	 * \return The id of the item.
	 */
	[[nodiscard]]
	uint32_t getItemId() const;

	/**
	 * \brief Gets the position of the item.
	 * \return The position of the item.
	 */
	[[nodiscard]]
	Position getPosition() const;
protected:
	uint32_t m_uid{};
	uint32_t m_itemId;
	Position m_position{};
};
