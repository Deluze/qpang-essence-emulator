#pragma once

#include "qpang/Game.h"
#include "qpang/chat/command/Command.h"

class DisconnectCommand : public Command
{
public:
	DisconnectCommand() : Command(3)
	{
		
	}

	std::vector<CommandArgument*> getArguments() override
	{
		return {
			Command::argTypes[Command::Validation::STRING]
		};
	}

	void handle(std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{
		auto nickname = args.at(0);

		auto target = Game::instance()->getOnlinePlayer(nickname);

		if (target == nullptr)
		{
			player->broadcast(u"This player is not online.");
			return;
		}

		target->close();
		player->broadcast(u"Done!");
	}
};