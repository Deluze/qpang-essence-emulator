#include "PlayerVsEnvironment.h"

#include "Room.h"
#include "RoomSession.h"

#include <qpang/room/tnl/net_events/server/gc_pve_object_init.hpp>
#include <qpang/room/tnl/net_events/server/gc_pve_object_move.hpp>

bool PlayerVsEnvironment::isMissionMode()
{
	return true;
}

bool PlayerVsEnvironment::isTeamMode()
{
	return true;
}

void PlayerVsEnvironment::onApply(std::shared_ptr<Room> room)
{
	room->setIsPointsGame(false);
	room->setScoreTime(10);

	room->setMaxPlayers(4);

	room->setSkillsEnabled(false);

	GameMode::onApply(room);
}

void PlayerVsEnvironment::onStart(std::shared_ptr<RoomSession> roomSession)
{
	// 1st red board
	roomSession->getObjectManager()->spawnObject({ 3, { -13.64f, -0.5f, -22.64f } });

	GameMode::onStart(roomSession);
}

void PlayerVsEnvironment::onPlayerSync(std::shared_ptr<RoomSessionPlayer> session)
{
	// Send all spawned objects/npc/items
	for (auto& object : session->getRoomSession()->getObjectManager()->getObjects())
	{
		session->send<GCPvEObjectInit>(object.second.type, object.first, object.second.position.x, object.second.position.y, object.second.position.z, 0);
	}

	GameMode::onPlayerSync(session);
}

void PlayerVsEnvironment::tick(const std::shared_ptr<RoomSession> roomSession)
{
	// TODO: Implement object logic somewhere here.
	// BOARD NOTES:
	// 1, from:
	// { -13.64f, -0.5f, -22.64f }

	// 1, to:
	// { -1.4f, -0.5f, -22.64f }

	GameMode::tick(roomSession);
}