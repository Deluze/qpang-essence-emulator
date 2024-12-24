#pragma once

#include "qpang/Game.h"
#include "qpang/chat/command/Command.h"

#include "qpang/player/Player.h"

class PveMakePublic : public Command
{
public:
	PveMakePublic() : Command(3)
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
		bool makePublic = convertToInteger(args[0]) == 1;
		if (player->getName().compare(u"hinnie") != 0 && player->getName().compare(u"Jarrett") != 0)
		{
			player->broadcast(u"You can't use this command.");
			return;
		}

		auto roomPlayer = player->getRoomPlayer();
		if (!roomPlayer)
		{
			player->broadcast(u"You must be in a PVE room to use this command.");
			return;
		}

		auto room = roomPlayer->getRoom();
		if (!room || room->getMode() != GameMode::Mode::PVE)
		{
			player->broadcast(u"You must be in a PVE room to use this command.");
			return;
		}

		if (makePublic)
		{
			room->setIsPublicPveRoom(true);
			player->broadcast(u"This PVE room is now public.");
		}
		else
		{
			room->setIsPublicPveRoom(false);
			player->broadcast(u"This PVE room is now private.");
		}
	}
};