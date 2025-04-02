#pragma once

#include "qpang/player/Player.h"
#include "qpang/room/RoomPlayer.h"
#include "qpang/room/Room.h"
#include "qpang/room/session/RoomSession.h"
#include <qpang/Game.h>

#include "qpang/chat/command/Command.h"

#include <string.h>
#include <qpang/room/tnl/net_events/server/gc_game_state.hpp>
#include <qpang/room/tnl/net_events/server/gc_card.hpp>

class AddSkillPointsCommand : public Command
{
public:
	AddSkillPointsCommand() : Command(3)
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
		const auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
		{
			player->broadcast(u"Enter a game room to execute this command.");

			return;
		}

		const auto roomSessionPlayer = roomPlayer->getRoomSessionPlayer();

		if (roomSessionPlayer == nullptr)
		{
			player->broadcast(u"Start playing a game to execute this command.");

			return;
		}

		const auto roomSession = roomSessionPlayer->getRoomSession();

		if (roomSession == nullptr)
		{
			player->broadcast(u"The room is not in session. Start playing to execute this command.");

			return;
		}

		const auto points = convertToInteger(args.at(0));

		roomSessionPlayer->getSkillManager()->addSkillPoints(points);

		player->broadcast(u"[Server] Added skillpoints.");

	}
};