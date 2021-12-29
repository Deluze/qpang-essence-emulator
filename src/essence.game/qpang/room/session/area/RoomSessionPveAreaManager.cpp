#include "RoomSessionPveAreaManager.h"

#include <iostream>

#include "AABBHelper.h"

#include "gc_pve_area_trigger_init.hpp"

void RoomSessionPveAreaManager::initialize(const std::shared_ptr<RoomSession>& roomSession)
{
	m_roomSession = roomSession;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void RoomSessionPveAreaManager::tick() const
{
	// Note: See cg_move_report.
}

void RoomSessionPveAreaManager::initializeAreas()
{
	const auto roomSession = m_roomSession.lock();

	if (roomSession == nullptr)
	{
		return;
	}

	m_areas.clear();

	const auto mapId = roomSession->getPveRoundManager()->getMap();
	const auto areaData = Game::instance()->getPveManager()->getAreaDataByMapId(mapId);

	for (const auto& data : areaData)
	{
		const auto area = PveArea(data);

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

	roomSession->relayPlaying<GCPvEAreaTriggerInit>(area);

	m_areas[area->getUid()] = area;
}

std::vector<std::shared_ptr<PveArea>> RoomSessionPveAreaManager::getAreas()
{
	std::vector<std::shared_ptr<PveArea>> areas{};

	for (const auto& [id, area] : m_areas)
	{
		areas.push_back(area);
	}

	return areas;
}

void RoomSessionPveAreaManager::removeAll()
{
	m_areas.clear();
}

void RoomSessionPveAreaManager::onAreaTrigger(const uint32_t areaId, const uint32_t playerId)
{
}

void RoomSessionPveAreaManager::onPlayerSync(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer) const
{
	for (const auto& [id, area] : m_areas)
	{
		roomSessionPlayer->send<GCPvEAreaTriggerInit>(area);
	}
}

void RoomSessionPveAreaManager::onStart()
{
	initializeAreas();
}
