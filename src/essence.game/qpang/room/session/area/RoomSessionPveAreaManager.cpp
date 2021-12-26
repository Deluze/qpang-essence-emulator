#include "RoomSessionPveAreaManager.h"

#include "AABBHelper.h"

#include <iostream>

#include "RoomSession.h"
#include "RoomSessionPlayer.h"

#include "gc_pve_area_trigger_init.hpp"

void RoomSessionPveAreaManager::initialize(const std::shared_ptr<RoomSession>& roomSession)
{
	m_roomSession = roomSession;

	createAreas();
}

void RoomSessionPveAreaManager::createAreas()
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	const std::vector areas
	{
		PveArea(PveArea::Bound{ 16.050f, -38.47f }, PveArea::Bound{ 25.30f, -29.032320f })
	};

	// TODO: Get all areas from the PveManager and create them (pve manager has yet to be made).
	for (const auto& area : areas)
	{
		createArea(std::make_shared<PveArea>(area));
	}
}

void RoomSessionPveAreaManager::createArea(const std::shared_ptr<PveArea>& area)
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	area->setId(m_areas.size() + 1);

	roomSession->relayPlaying<GCPvEAreaTriggerInit>(area);

	m_areas[static_cast<uint32_t>(1)] = area;
}

std::shared_ptr<PveArea> RoomSessionPveAreaManager::findAreaById(const uint32_t id)
{
	const auto& it = m_areas.find(id);

	if (it == m_areas.end())
	{
		return nullptr;
	}

	return it->second;
}

void RoomSessionPveAreaManager::onAreaTrigger(const uint32_t areaId, const uint32_t playerId)
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	const auto sessionPlayer = roomSession->find(playerId);

	if (sessionPlayer == nullptr)
	{
		return;
	}

	const auto& area = findAreaById(areaId);

	if (area == nullptr)
	{
		return;
	}

	// If the area has already been initialized, return.
	if (area->isInitialized())
	{
		return;
	}

	// The client tells us it has triggered the area, but lets check on the server to make sure.
	if (!AABBHelper::isBetweenAreaBounds(area->getMinBound(), area->getMaxBound(), sessionPlayer->getPosition()))
	{
		return;
	}

	area->setIsInitialized(true);

	std::cout << "Area " << areaId << " has been initialized." << std::endl;

	// TODO: Spawn npcs with this area id.
}

void RoomSessionPveAreaManager::onPlayerSync(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer) const
{
	for (const auto& [id, area] : m_areas)
	{
		roomSessionPlayer->send<GCPvEAreaTriggerInit>(area);
	}
}
