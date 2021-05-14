#pragma once

#include "qpang/Game.h"
#include "qpang/chat/command/Command.h"

#include "qpang/room/session/RoomSession.h"
#include "qpang/room/RoomPlayer.h"

class LeaderboardCommand : public Command
{
public:
	LeaderboardCommand() : Command(3)
	{

	}

	void handle(std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{
		Game::instance()->getLeaderboard()->refresh();
	}
};