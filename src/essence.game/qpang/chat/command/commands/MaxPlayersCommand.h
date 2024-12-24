#pragma once

#include "qpang/chat/command/Command.h"

#include "qpang/player/Player.h"
#include "qpang/room/RoomPlayer.h"
#include "qpang/room/Room.h"

// This command is disabled.
class MaxPlayersCommand final : public Command
{
public:
	MaxPlayersCommand() : Command(3)
	{
	}

	void handle(const std::shared_ptr<Player> player, const std::vector<std::u16string>& args) override
	{
		const auto roomPlayer = player->getRoomPlayer();

		if (roomPlayer == nullptr)
		{
			player->broadcast(u"You need to be in a room");

			return;
		}

		roomPlayer->getRoom()->setMaxPlayers(16);

		player->broadcast(u"A maximum of 16 players can now enter the room!");
	}
};