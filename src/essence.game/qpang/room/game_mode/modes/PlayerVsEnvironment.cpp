#include "PlayerVsEnvironment.h"

#include "Room.h"
#include "RoomSession.h"


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

void PlayerVsEnvironment::tick(const std::shared_ptr<RoomSession> roomSession)
{
	GameMode::tick(roomSession);
}