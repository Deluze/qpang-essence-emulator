#pragma once

#include <memory>

#include "PveItemData.h"
#include "RoomSessionPlayer.h"

class RoomSession;

class PveItem final
{
public:
	PveItem() = default;

	explicit PveItem(PveItemData data);
	PveItem(eItemSpawnType spawnType, const Position& position);
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
	 * \brief Sets the item id for the item.
	 */
	void setWeightedRandomItemId();

	void setIsPickedUp(bool value);

	void setLastPickupTime(uint32_t time);

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

	[[nodiscard]]
	eItemSpawnType getSpawnType() const;

	[[nodiscard]]
	bool isPickedUp() const;
protected:
	uint32_t m_uid{};

	uint32_t m_itemId{};
	eItemSpawnType m_spawnType;

	Position m_position{};

	uint32_t m_respawnTime = NULL;
	bool m_shouldRespawn = false;

	uint32_t m_lastPickupTime = NULL;
	bool m_isPickedUp = false;

	std::vector<WeightedItem> m_weightedCoins
	{
		WeightedItem { eItemId::GOLDEN_COIN, 3 },
		WeightedItem { eItemId::SILVER_COIN, 7 },
		WeightedItem { eItemId::BRONZE_COIN, 10 }
	};

	std::vector<WeightedItem> m_weightedAmmoAndMedkits
	{
		WeightedItem { eItemId::AMMO_CLIP, 1 },
		WeightedItem { eItemId::RED_MEDKIT, 1 }
	};
};
