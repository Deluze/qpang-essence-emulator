#pragma once

#include <cstdint>
#include <vector>

#include "Position.h"
#include "PveAreaData.h"
#include "RoomSessionPlayer.h"

class PveArea
{
public:
	explicit PveArea(const PveAreaData& pveAreaData);

	/**
	 * \brief Initializes the area by spawning npcs.
	 */
	void initialize(const std::shared_ptr<RoomSession>& roomSession);

#pragma region Setters
#pragma endregion

#pragma region Getters
	[[nodiscard]]
	uint32_t getUid() const;

	[[nodiscard]]
	Position getMinBound() const;

	[[nodiscard]]
	Position getMaxBound() const;

	[[nodiscard]]
	uint32_t getFloorNumber() const;

	[[nodiscard]]
	bool isInitialized() const;
#pragma endregion

#pragma region Event handlers
	/**
	 * \brief Gets triggered upon a player entering an area.
	 * \param roomSessionPlayer The player that has triggered the event.
	 */
	void onAreaEnter(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer);

	/**
	 * \brief Gets triggered upon a player exiting an area.
	 * \param roomSessionPlayer The player that has triggered the event.
	 */
	void onAreaExit(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer);
#pragma endregion
private:
	uint32_t m_uid{};

	Position m_minBound;
	Position m_maxBound;

	uint32_t m_floorNumber;

	bool m_isInitialized;

	std::vector<uint32_t> m_players{};
};
