#pragma once

#include "qpang/Game.h"
#include "qpang/chat/command/Command.h"

#include "qpang/player/Player.h"

class UnmuteCommand : public Command
{
public:
	UnmuteCommand() : Command(4)
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
		const auto nickname = args.at(0);

		auto target = Game::instance()->getPlayer(nickname);

		if (target == nullptr)
		{
			player->broadcast(u"This player does not exist.");
			return;
		}

		if (!target->isMuted())
		{
			player->broadcast(u"This player is not muted.");
			return;
		}

		target->unmute();
		player->broadcast(u"This player is now unmuted.");
	}
};