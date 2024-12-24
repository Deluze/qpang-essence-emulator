#pragma once

#include "qpang/Game.h"
#include "qpang/chat/command/Command.h"

class BroadcastCommand : public Command
{
public:
	BroadcastCommand() : Command(4)
	{

	}

	std::vector<CommandArgument*> getArguments() override
	{
		return {
			Command::argTypes[Command::Validation::STRING],
		};
	}

	void handle(std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{
		const auto &message = args.at(0);

		if (message.empty()) {
			player->broadcast(u"The message you are attempting to broadcast is empty.");

			return;
		}

		Game::instance()->broadcast(u"Announcement: " + message);
	}
};