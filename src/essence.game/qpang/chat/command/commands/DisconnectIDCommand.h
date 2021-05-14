#pragma once

#include "qpang/Game.h"
#include "qpang/chat/command/Command.h"

class DisconnectIDCommand : public Command
{
public:
	DisconnectIDCommand() : Command(3)
	{
		
	}

	std::vector<CommandArgument*> getArguments() override
	{
		return {
			Command::argTypes[Command::Validation::INTEGER],
		};
	}

	void handle(std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{
		const auto id = convertToInteger(args.at(0));

		auto target = Game::instance()->getOnlinePlayer(id);

		if (target == nullptr)
		{
			player->broadcast(u"This player is not online.");
			return;
		}

		target->close();
		player->broadcast(u"Done!");
	}
};