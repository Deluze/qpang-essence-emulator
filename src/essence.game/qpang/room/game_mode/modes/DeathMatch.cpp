#include "DeathMatch.h"
#include "Room.h"
#include "Player.h"
#include "RoomPlayer.h"

void DeathMatch::onApply(std::shared_ptr<Room> room)
{
	for (const auto &player : room->getPlayers())
	{
		player.second->setTeam(0);
	}

	GameMode::onApply(room);
}
