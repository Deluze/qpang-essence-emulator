#pragma once

#include <memory>
#include <map>

#include "Position.h"

class RoomSession;

class RoomSessionPveItemManager
{
public:
	struct Item
	{
		uint32_t id;
		uint32_t uid;
		Position position;
	};

	/**
	 * \brief Initializes the pve item manager with the room session.
	 * \param roomSession The given room session.
	 */
	void initialize(const std::shared_ptr<RoomSession>& roomSession);
private:
	std::weak_ptr<RoomSession> m_roomSession;
};