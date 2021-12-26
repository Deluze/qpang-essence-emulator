#pragma once

#include <memory>
#include <map>

#include "PveArea.h"

class RoomSession;
class RoomSessionPlayer;

class RoomSessionPveAreaManager
{
public:
	/**
	 * \brief Initializes the pve area manager with the room session.
	 * \param roomSession The room in session.
	 */
	void initialize(const std::shared_ptr<RoomSession>& roomSession);

	/**
	 * \brief Creates all the areas.
	 */
	void createAreas();

	/**
	 * \brief Creates an area.
	 * \param area The area to create.
	 */
	void createArea(const std::shared_ptr<PveArea>& area);

	/**
	 * \brief Looks up the area by id.
	 * \param id The id of the area.
	 * \return The found pve area otherwise nullptr.
	 */
	std::shared_ptr<PveArea> findAreaById(uint32_t id);

#pragma region NetEvent handlers
	/**
	 * \brief Gets called when a player triggers an area by walking into it.
	 * \param areaId The id of the area that has been triggered.
	 * \param roomSessionPlayer The player that has triggered the area.
	 */
	void onAreaTrigger(uint32_t areaId, uint32_t playerId);

	void onPlayerSync(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer) const;
#pragma endregion

private:
	std::weak_ptr<RoomSession> m_roomSession;
	std::map<uint32_t, std::shared_ptr<PveArea>> m_areas;
};