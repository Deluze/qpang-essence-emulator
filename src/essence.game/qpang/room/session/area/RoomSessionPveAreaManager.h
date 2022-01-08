#pragma once

#include <memory>
#include <map>
#include <mutex>

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

	void tick() const;

	/**
	 * \brief Creates all the areas.
	 */
	void initializeAreas();

	/**
	 * \brief Retrieves all areas that are required to have been passed through.
	 * \return All areas that must've been passed through.	
	 */
	std::vector<std::shared_ptr<PveArea>> getAllRequiredPassThroughAreas();

	/**
	 * \brief Creates an area.
	 * \param area The area to create.
	 */
	void createArea(const std::shared_ptr<PveArea>& area);

	std::vector<std::shared_ptr<PveArea>> getAreas();

	void removeAll();

#pragma region NetEvent handlers
	void onStart();
#pragma endregion

private:
	std::recursive_mutex m_mx;
	std::weak_ptr<RoomSession> m_roomSession;
	std::map<uint32_t, std::shared_ptr<PveArea>> m_areas;
};