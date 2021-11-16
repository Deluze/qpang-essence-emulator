#pragma once

#include "qpang/chat/command/Command.h"
#include "qpang/room/RoomPlayer.h"

class DebugCommand final : public Command
{
public:
	DebugCommand() : Command(3)
	{
	}

	void handle(const std::shared_ptr<Player> player, const std::vector<std::u16string>& args) override
	{
		const auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
		{
			player->broadcast(u"You need to be in a room in order to use this command.");

			return;
		}

		const auto roomSessionPlayer= roomPlayer->getRoomSessionPlayer();

		if (roomSessionPlayer == nullptr)
		{
			player->broadcast(u"You need to be in a game in order to use this command.");

			return; 
		}


	}
};