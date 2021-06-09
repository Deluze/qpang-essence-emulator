#pragma once

#include "qpang/chat/command/Command.h"

#include "qpang/player/Player.h"
#include "qpang/room/RoomPlayer.h"
#include "qpang/room/Room.h"

// This command is disabled.
class UnlimitedCommand: public Command
{
public:
	UnlimitedCommand() : Command(3)
	{

	}

	void handle(std::shared_ptr<Player> player, const std::vector<std::u16string>& args)
	{
		auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
		{
			player->broadcast(u"U need to be in a room");
			return;
		}

		roomPlayer->getRoom()->setMaxPlayers(127);

		player->broadcast(u"A maximum of 127 players can now enter the room!");
	}
};