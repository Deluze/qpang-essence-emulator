#pragma once

#include "qpang/Game.h"
#include "qpang/chat/command/Command.h"

#include "qpang/player/Player.h"

class MuteCommand : public Command
{
public:
	MuteCommand() : Command(4)
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
		auto target = Game::instance()->getPlayer(args.at(0));

		if (target == nullptr)
		{
			player->broadcast(u"This player does not exist");
			return;
		}

		if (target->isMuted())
		{
			player->broadcast(u"This player is already muted");
			return;
		}

		target->mute();

		player->broadcast(u"This player is now muted");
	}
};