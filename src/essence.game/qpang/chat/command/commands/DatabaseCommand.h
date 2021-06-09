#pragma once

#include "qpang/Game.h"
#include "qpang/chat/command/Command.h"

class DatabaseCommand : public Command
{
public:
	DatabaseCommand() : Command(3)
	{

	}

	void handle(std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{
		DATABASE_DISPATCHER->executeAll();

		player->broadcast(u"Executed all database queries.");
	}
};