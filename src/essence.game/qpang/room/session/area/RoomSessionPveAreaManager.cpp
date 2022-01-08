#include "RoomSessionPveAreaManager.h"

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
	std::lock_guard<std::recursive_mutex> lg(m_mx);

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

	std::lock_guard<std::recursive_mutex> lg(m_mx);

	m_areas[area->getUid()] = area;
}

std::vector<std::shared_ptr<PveArea>> RoomSessionPveAreaManager::getAreas()
{
	std::lock_guard<std::recursive_mutex> lg(m_mx);

	std::vector<std::shared_ptr<PveArea>> areas{};

	for (const auto& [id, area] : m_areas)
	{
		areas.push_back(area);
	}

	return areas;
}

void RoomSessionPveAreaManager::removeAll()
{
	std::lock_guard<std::recursive_mutex> lg(m_mx);

	m_areas.clear();
}

void RoomSessionPveAreaManager::onStart()
{
	initializeAreas();
}
