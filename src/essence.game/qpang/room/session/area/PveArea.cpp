#include "PveArea.h"

#include "Player.h"
#include "RoomSession.h"

PveArea::PveArea(const PveAreaData& pveAreaData) :
	m_uid(pveAreaData.uid),
	m_minBound(pveAreaData.minBound),
	m_maxBound(pveAreaData.maxBound),
	m_floorNumber(pveAreaData.floorNumber),
	m_passThroughIsRequired(pveAreaData.passThroughIsRequired),
	m_isInitialized(false)
{
}

void PveArea::initialize(const std::shared_ptr<RoomSession>& roomSession)
{
	m_isInitialized = true;

	roomSession->getNpcManager()->spawnNpcsForArea(m_uid);
}

uint32_t PveArea::getUid() const
{
	return m_uid;
}

Position PveArea::getMinBound() const
{
	return m_minBound;
}

Position PveArea::getMaxBound() const
{
	return m_maxBound;
}

uint32_t PveArea::getFloorNumber() const
{
	return m_floorNumber;
}

bool PveArea::isPassThroughRequired() const
{
	return m_passThroughIsRequired;
}

bool PveArea::isInitialized() const
{
	return m_isInitialized;
}

void PveArea::onAreaEnter(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer)
{
	if (roomSessionPlayer->getRoomSession()->getElapsedTime() < 1000)
	{
		return;
	}

	const auto playerId = roomSessionPlayer->getPlayer()->getId();

	const auto& it = std::find(m_players.begin(), m_players.end(), playerId);

	// Check if player is already in the area.
	if (it != m_players.end())
	{
		return;
	}

	m_players.push_back(playerId);

	if (!m_isInitialized)
	{
		initialize(roomSessionPlayer->getRoomSession());
	}
}

void PveArea::onAreaExit(const std::shared_ptr<RoomSessionPlayer>& roomSessionPlayer)
{
	const auto playerId = roomSessionPlayer->getPlayer()->getId();

	const auto& it = std::find(m_players.begin(), m_players.end(), playerId);

	// Player is not in the area.
	if (it == m_players.end())
	{
		return;
	}

	m_players.erase(it);

	//printf("[-] Player %u has exited area %u\n", playerId, m_uid);
}
