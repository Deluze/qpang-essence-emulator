#pragma once

#include "qpang/Game.h"
#include "qpang/chat/command/Command.h"

class BanCommand : public Command
{
public:
	BanCommand() : Command(4)
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

		auto target = Game::instance()->getPlayer(nickname);

		if (target == nullptr)
		{
			player->broadcast(u"This player does not exist.");
			return;
		}

		if (player->getRank() == 4 && target->getRank() == 3) {
			player->broadcast(u"You are not permitted to ban a game master.");

			return;
		}

		auto bannedByUserId = player->getUserId();

		const auto currTime = time(NULL);
		target->ban(currTime + (60 * 60 * 24), bannedByUserId);

		player->broadcast(u"Player has been banned for 24 hours.");
	}
};