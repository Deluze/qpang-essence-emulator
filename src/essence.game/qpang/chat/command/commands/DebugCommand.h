#pragma once

#include "qpang/chat/command/Command.h"
#include "qpang/room/RoomPlayer.h"

#include <qpang/room/tnl/net_events/server/gc_pve_object_init.hpp>

class DebugCommand final : public Command
{
public:
	DebugCommand() : Command(3)
	{
	}

	std::vector<CommandArgument*> getArguments() override
	{
		return {
			//Command::argTypes[Command::Validation::INTEGER],
			//Command::argTypes[Command::Validation::INTEGER]
			Command::argTypes[Command::Validation::STRING],
			Command::argTypes[Command::Validation::STRING],
			Command::argTypes[Command::Validation::STRING],
			Command::argTypes[Command::Validation::STRING]
		};
	}

	void handle(const std::shared_ptr<Player> player, const std::vector<std::u16string>& args) override
	{
		const auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
		{
			player->broadcast(u"You need to be in a room in order to use this command.");

			return;
		}

		const auto roomSessionPlayer = roomPlayer->getRoomSessionPlayer();

		if (roomSessionPlayer == nullptr)
		{
			player->broadcast(u"You need to be in a game in order to use this command.");

			return;
		}

		const auto roomSession = roomSessionPlayer->getRoomSession();

		auto a1 = std::stof(std::string(args[0].begin(), args[0].end()));
		auto a2 = std::stof(std::string(args[1].begin(), args[1].end()));
		auto a3 = std::stof(std::string(args[2].begin(), args[2].end()));
		auto a4 = std::stof(std::string(args[3].begin(), args[3].end()));

		roomSessionPlayer->send<GCPvEObjectInit>(a1, 50, a2, a3, a4, 0);
		//roomSessionPlayer->send<GCPvEObjectMove>(3, 39.1f, 0.f + convertToInteger(args[0]), -5.1f, 1000);
	}
};