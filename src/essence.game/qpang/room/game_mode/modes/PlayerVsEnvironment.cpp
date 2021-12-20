#include "PlayerVsEnvironment.h"

#include "Room.h"
#include "RoomSession.h"

#include "RedBoardObject.h"

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
	roomSession->getObjectManager()->spawnObject(std::make_unique<RedBoardObject>(eRedBoardActionId::STAGE0_PIT1_BOARD1, Position { -13.64f, -0.5f, -22.64f }));

	GameMode::onStart(roomSession);
}

void PlayerVsEnvironment::onPlayerSync(std::shared_ptr<RoomSessionPlayer> session)
{
	// Send all spawned objects/npc/items
	session->getRoomSession()->getObjectManager()->onPlayerSync(session);
	session->getRoomSession()->getNpcManager()->onPlayerSync(session);
	session->getRoomSession()->getPveItemManager()->onPlayerSync(session);

	GameMode::onPlayerSync(session);
}

void PlayerVsEnvironment::tick(std::shared_ptr<RoomSession> roomSession)
{
	roomSession->getObjectManager()->tick(roomSession);
	roomSession->getNpcManager()->tick(roomSession);
	roomSession->getPveItemManager()->tick(roomSession);

	GameMode::tick(roomSession);
}