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

	void tick() const;

	/**
	 * \brief Creates all the areas.
	 */
	void initializeAreas();

	/**
	 * \brief Creates an area.
	 * \param area The area to create.
	 */
	void createArea(const std::shared_ptr<PveArea>& area);

	void removeAll();

#pragma region NetEvent handlers
	void onAreaTrigger(uint32_t areaId, uint32_t playerId);

	void onPlayerSync(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer) const;

	void onStart();
#pragma endregion

private:
	std::weak_ptr<RoomSession> m_roomSession;
	std::map<uint32_t, std::shared_ptr<PveArea>> m_areas;
};